#include "FrameManager.hpp"

MyFrame::MyFrame(const wxString& title, const wxPoint& position, const wxSize& size) :
	wxFrame(nullptr, wxID_ANY, title, position, size)
{
	splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxSP_BORDER | wxSP_LIVE_UPDATE);

	control = new ControlPanel(splitter);
	control->BindButtons(main_functions);
	
	content = new wxPanel(splitter);

	control->SetBackgroundColour(wxColor("#FF6700"));
	content->SetBackgroundColour(wxColor("#EBEBEB"));

	splitter->SetMinimumPaneSize(200);
	splitter->SplitVertically(control, content);
	splitter->SetSashPosition(size.GetWidth() / 4, true);
}

void MyFrame::ShowMainGenerateForm(wxCommandEvent& event)
{
	deletePanel();
    mainGenerateForm = new GenerateFormPanel(splitter, wxCommandEventFunction(&MyFrame::GenerateSequence));
    splitter->ReplaceWindow(splitter->GetWindow2(), mainGenerateForm);
    mainGenerateForm->Raise();
}

void MyFrame::GenerateSequence()
{
	wxString length = mainGenerateForm->getLength();
	wxString generator = mainGenerateForm->getGenerator();
	if (ActionManager::CheckEnteredLength(length)) {
		ActionManager::GenerateRandomBits(length, generator, main_sequence);
		deletePanel();
		control->EnableButtonsSaveAndTest();
		showSequence = new ShowSequencePanel(splitter, main_sequence);
		splitter->ReplaceWindow(splitter->GetWindow2(), showSequence);
		showSequence->Raise();
	}
}

void MyFrame::LoadMainFromFile(wxCommandEvent& event)
{
	LoadFromFile(main_sequence, wxString("загрузка последовательности"));

	deletePanel();
	control->EnableButtonsSaveAndTest();
	showSequence = new ShowSequencePanel(splitter, main_sequence);
	splitter->ReplaceWindow(splitter->GetWindow2(), showSequence);
	showSequence->Raise();
}

void MyFrame::TestMainSequence(wxCommandEvent& event)
{
	main_results.clear();
	main_results.push_back({ "частотный тест", 1, ActionManager::FrequencyTest(main_sequence) });
	main_results.push_back({ "Тест на последовательность одинаковых бит", 1, ActionManager::TestForASequenceOfIdenticalBits(main_sequence) });
	int i = 1;
	for (const bool tmp : ActionManager::extendedTestForArbitraryDeviations(main_sequence)) {
		main_results.push_back({ "Расширенный тест на произвольные отклонения" , i++, tmp });
	}

	deletePanel();
	control->EnableLastBtn();
	showTest = new ShowTestPanel(splitter, main_results, wxString("результаты тестов"));
	splitter->ReplaceWindow(splitter->GetWindow2(), showTest);
	showTest->SetColumnWidths();
	showTest->Raise();
}

void MyFrame::CompareSequences(wxCommandEvent& event)
{
	LoadFromFile(second_sequence, wxString("загрузить последовательность для сравнения"));
	
	second_results.clear();
	second_results.push_back({ "частотный тест", 1, ActionManager::FrequencyTest(second_sequence) });
	second_results.push_back({ "Тест на последовательность одинаковых бит", 1, ActionManager::TestForASequenceOfIdenticalBits(second_sequence) });
	int i = 1;
	for (const bool tmp : ActionManager::extendedTestForArbitraryDeviations(second_sequence)) {
		second_results.push_back({ "Расширенный тест на произвольные отклонения" , i++, tmp });
	}

	deletePanel();
	compareResults = new CompareResultsPanel(splitter, main_results, second_results);
	splitter->ReplaceWindow(splitter->GetWindow2(), compareResults);
	compareResults->setWidths();
	compareResults->Raise();
}

void MyFrame::OnClose(wxCommandEvent& event)
{
	wxMessageDialog dialog(this, "вы действительно хотите выйти?",
		"выход", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if (dialog.ShowModal() == wxID_YES) {
		this->Close(true);
	}
}

void MyFrame::deletePanel()
{
	wxWindow* oldWindow = splitter->GetWindow2();
	if (oldWindow) {
		oldWindow->Destroy();
	}
}

void MyFrame::LoadFromFile(vector<bool>& seq, const wxString& title)
{
	wxFileDialog openFileDialog(this, title,
		"", // путь по умолчанию
		"", // имя файла по умолчанию
		_("Текстовые файлы (*.txt)|*.txt"), // типы файлов
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK) {
		seq.clear();
		seq.resize(0);

		wxString filePath = openFileDialog.GetPath();
		wxFileName fileName(filePath);

		ifstream inFile(fileName.GetFullPath().ToStdString());

		if (inFile.is_open()) {
			string line;
			getline(inFile, line);

			if (!line.empty()) {

				int size = 0;
				for (char c : line) {
					if (c != '0' && c != '1') {
						wxMessageDialog dialog(nullptr, _("Файл не содержит последовательности. Последовательность не загружена."),
							_("Ошибка"), wxOK | wxICON_ERROR);
					}
					else {
						size++;
					}
				}

				wxProgressDialog* dialog = new wxProgressDialog("чтение последовательности из файла", "чтение файла",
					size, this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);
				for (int i = 0; i < size; i++) {
					seq.push_back(line[i] == '1');
					dialog->Update(i, wxString::Format(wxT("%i"), i), NULL);
				}
				dialog->Update(size);

				if (!seq.empty()) {
					return;
				}
				else {
					wxMessageDialog dialog(nullptr, _("Пустая строка в файле. Последовательность не загружена."),
						_("Ошибка"), wxOK | wxICON_ERROR);
					dialog.ShowModal();
				}
			}
			else {
				wxMessageDialog dialog(nullptr, _("Файл пуст. Последовательность не загружена."),
					_("Ошибка"), wxOK | wxICON_ERROR);
				dialog.ShowModal();
			}

			inFile.close();
		}
		else {
			wxMessageDialog dialog(nullptr, _("Ошибка при открытии файла для чтения."),
				_("Ошибка"), wxOK | wxICON_ERROR);
			dialog.ShowModal();
		}
	}
	else {
		return;
	}
}

void MyFrame::SaveToFile(wxCommandEvent& event)
{
	wxFileDialog saveFileDialog(this, _("Сохранить последовательность"),
		"", // путь по умолчанию
		"sequence", // имя файла по умолчанию
		_("Текстовые файлы (*.txt)|*.txt"), // типы файлов
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_FILE_MUST_EXIST);

	if (saveFileDialog.ShowModal() == wxID_OK) {
		wxString filePath = saveFileDialog.GetPath();
		ofstream outFile(filePath.ToStdString(), ios::out);

		if (outFile.is_open()) {
			string seqAsStr = "";
			wxProgressDialog* dialog = new wxProgressDialog("Запись последовательности в файл", "запись в файл",
				main_sequence.size(), this, wxPD_AUTO_HIDE | wxPD_APP_MODAL);
			for (int i = 0; i < main_sequence.size(); i++) {
				outFile << (main_sequence[i] ? "1" : "0");
				dialog->Update(i, wxString::Format(wxT("%i"), i), NULL);
			}
			dialog->Update(main_sequence.size());
			outFile.close();

			wxMessageBox(("Последовательность успешно сохранена!"),
				_("Информация"));
		}
		else {
			wxMessageDialog dialog(nullptr, "Ошибка при открытии файла для записи.",
				"Ошибка", wxOK | wxICON_ERROR);
			dialog.ShowModal();
		}
	}
}
