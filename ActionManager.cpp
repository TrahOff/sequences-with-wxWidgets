#include "ActionManager.hpp"

bool ActionManager::CheckEnteredLength(wxString value)
{
	int sequenceLength = 0;
	if (value.empty()) {
		wxMessageDialog dialog(nullptr, "Пожалуйста, введите длину последовательности.",
			"Ошибка", wxOK | wxICON_ERROR);
		dialog.SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT));
		dialog.ShowModal();
		return false;
	}
	else {
		try {
			sequenceLength = stoi(value.ToStdString());
		}
		catch (const invalid_argument& e) {
			wxMessageDialog dialog(nullptr, "Некорректный ввод. Пожалуйста, введите целое число.",
				"Ошибка", wxOK | wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}

		if (sequenceLength < 1) {
			wxMessageDialog dialog(nullptr, "Длина последовательности должна быть больше 0.",
				"Ошибка", wxOK | wxICON_ERROR);
			dialog.ShowModal();
			return false;
		}
	}
	return true;
}

void ActionManager::GenerateRandomBits(wxString value, wxString generator, vector<bool>& sequence)
{
	srand(time(nullptr));
	int size = stoi(value.ToStdString());

	sequence.clear();
	sequence.resize(0);
	sequence.reserve(size);

	if (generator.ToStdString() == "Квадратичный конгруэнтный генератор") {
		QuadraticCongruentGenerator(sequence, size);
	}
	else if (generator.ToStdString() == "Генератор BBS") {
		BBSGenerator(sequence, size);
	}
	else {
		Yarrow_160(sequence, size);
	}
}

void ActionManager::QuadraticCongruentGenerator(vector<bool>& sequence, int size)
{
	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("генерация последовательности: Квадратичный конгруэнтный генератор",
		"Генерация", size, topWindow, wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	unsigned long long seed = rand(), a = 1366, b = 1283, c = 6075, m = 8388608;
	unsigned long long Xprev = seed;

	for (int i = 0; i < size; i++) {
		sequence.push_back(Xprev % 2);
		Xprev = (static_cast<unsigned long long>(a * Xprev * Xprev + b * Xprev + c)) % m;
		Xprev >>= 1;
		dialog->Update(i, wxString::Format(wxT("%i"), i), NULL);
	}
	dialog->Update(size);

	delete dialog;
}

void ActionManager::BBSGenerator(vector<bool>& sequence, int size)
{
	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("генерация последовательности: генератор BBS", "Генерация", size, topWindow,
		wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	long long p = rand(), q = rand(), seed = 0;

	while (!PrimeNuber(p)) p = rand();
	while (!PrimeNuber(q) || p == q || q % 4 != 3) q = rand();

	long long n = p * q;
	seed = rand() % (n - 1) + 1;
	while (GCD(seed, n) != 1) seed = rand() % (n - 1) + 1;

	for (int i = 0; i < size; i++) {
		unsigned long long u = static_cast<unsigned long long>(pow(seed, 2)) % n;
		sequence.push_back(u & 1);
		seed = u;
		dialog->Update(i, wxString::Format(wxT("%i"), i), NULL);
	}
	dialog->Update(size);

	delete dialog;
}

void ActionManager::Yarrow_160(vector<bool>& sequence, int size)
{
	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("генерация последовательности: Yarrow-160", "Генерация", size, topWindow,
		wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	YarrowGenerator YG;
	for (int i = 0; i < size; i++) {
		sequence.push_back(YG.generate() % 2);
		dialog->Update(i, wxString::Format(wxT("%i"), i), NULL);
	}

	dialog->Update(size);

	delete dialog;
}

bool ActionManager::PrimeNuber(long long number)
{
	if (number <= 1) return false;
	if (number == 2) return true;
	if (number % 2 == 0) return false;

	for (long long i = 3; i < number; i++) {
		if (number % i == 0) return false;
	}
	return true;
}

unsigned long long ActionManager::GCD(unsigned long long a, unsigned long long b)
{
	if (b == 0) return a;
	return GCD(b, a % b);
}

bool ActionManager::FrequencyTest(vector<bool>& frequency)
{
	vector<int> X;
	long count = 0;
	double sum = 0;
	X.reserve(frequency.size());

	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("Частотный тест", "тест 1", frequency.size(), topWindow,
		wxPD_AUTO_HIDE | wxPD_APP_MODAL);

	for (const auto& bit : frequency) {
		count++;
		X.push_back(bit * 2 - 1);
		if (count % 2 == 0) dialog->Update(count / 2, wxString::Format(wxT("%i"), count / 2), NULL);
	}

	for (const auto& x : X) {
		count++;
		sum += x;
		if (count % 2 == 0) dialog->Update(count / 2, wxString::Format(wxT("%i"), count / 2), NULL);
	}
	dialog->Update(frequency.size());
	X.clear();
	return (abs(sum) / sqrt(frequency.size())) <= 1.82138636;
}

bool ActionManager::TestForASequenceOfIdenticalBits(vector<bool>& frequency)
{
	double frequency_of_units = 0;
	long count = 0;

	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("Тест 2: Тест на последовательность одинаковых бит", "тест 2", frequency.size(), topWindow,
		wxPD_AUTO_HIDE | wxPD_APP_MODAL);
	for (const auto& bit : frequency) {
		count++;
		frequency_of_units += bit;
		if (count % 2 == 0) dialog->Update(count / 2, wxString::Format(wxT("%i"), count / 2), NULL);
	}

	frequency_of_units /= frequency.size();

	int Vn = 1;
	for (int j = 0; j < frequency.size() - 1; j++) {
		count++;
		if (frequency[j] != frequency[j + 1]) {
			Vn += 1;
		}
		if (count % 2 == 0) dialog->Update(count / 2, wxString::Format(wxT("%i"), count / 2), NULL);
	}

	double statistic = abs(Vn - 2 * frequency.size() * frequency_of_units * (1 - frequency_of_units)) /
		(2 * sqrt(2 * frequency.size()) * frequency_of_units * (1 - frequency_of_units));
	dialog->Update(frequency.size());
	delete dialog;

	return statistic <= 1.82138636;
}

vector<bool> ActionManager::extendedTestForArbitraryDeviations(vector<bool>& frequency)
{
	vector<int> X, sums;
	map<int, int> counts;
	vector<bool> Y;
	int L = 0;
	long count = 0;

	wxWindow* topWindow = wxTheApp->GetTopWindow();
	wxProgressDialog* dialog = new wxProgressDialog("Тест 3: Расширенный тест на произвольные отклонения", "тест 3", frequency.size(), topWindow,
		wxPD_AUTO_HIDE | wxPD_APP_MODAL);
	for (const auto& bit : frequency) {
		count++;
		X.push_back(2 * bit - 1);
		if (count % 4 == 0) dialog->Update(count / 4, wxString::Format(wxT("%i"), count / 4), NULL);
	}

	sums.resize(X.size() + 2);
	for (int i = 1; i < X.size(); i++) {
		count++;
		sums[i] = sums[i - 1] + X[i - 1];
		if (sums[i] == 0) {
			L++;
		}
		if (count % 4 == 0) dialog->Update(count / 4, wxString::Format(wxT("%i"), count / 4), NULL);
	}

	for (int value : sums) {
		count++;
		if (-9 <= value && value <= 9 && value != 0) {
			counts[value]++;
		}
		if (count % 4 == 0) dialog->Update(count / 4, wxString::Format(wxT("%i"), count / 4), NULL);
	}

	for (const auto& pair : counts) {
		count++;
		Y.push_back(abs(pair.second - L) / sqrt(2 * L * (4 * abs(pair.first) - 2)) <= 1.82138636);
		if (Y.back() == false) {
			while (Y.size() != 18) Y.push_back(false);
			break;
		}
		if (count % 4 == 0) dialog->Update(count / 4, wxString::Format(wxT("%i"), count / 4), NULL);
	}
	dialog->Update(frequency.size());
	delete dialog;
	X.clear();
	sums.clear();

	return Y;
}
