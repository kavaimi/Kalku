#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <string.h>
#include <cstring>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

const int MAX_REC_NUM = 200; //максимальное число обрабатываемых записей
const int SCR_REC_NUM = 50; //число записей, одновременно отображаемых на экране
const int SCR_COL_NUM = 110; //ширина окна консоли
struct TCandle    //изделие (свечи)
{
	char name[25];
	int summ;
	float size;
};
struct TPrice   // цена материала
{
	float paraf;
	float kras;
	float nit;
};
struct TMats   // количество материала
{
	float paraf;
	float kras;
	float nit;
};
struct TCosts // налоги
{
	float prib;
	float izd;
	float nadb;
};

const char* fieldName[] = { "КОД","НАЗВАНИЕ ИЗДЕЛИЯ","СТОИМОСТЬ" };

int FindField = 0;
TCandle FindMan;
TPrice Price{ 8,3,0.4 };
TCosts Costs{ 0.1, 0.14, 0.2 };
const int MAX_PERSON_NUMBER = 50;


int loadFromFile(TCandle*, int); //ввод из текстового файла в массив
void putItem(TCandle*); //ввод полей записи с клавиатуры
int addItem(TCandle*, int);  //добавление новых элементов в конец массива
int showList(TCandle*, int); //просмотр всех элементов массива
int saveToFile(TCandle*, int); //вывод элементов массива в текстовый файл
int editItem(TCandle*, int);  //корректировка полей выбранного элемента
int deleteItem(TCandle*, int);  //удаление выбранного элемента
int findItems(TCandle*, int); //поиск
int saveToDan(TPrice*, TCosts*); // Ввод расходов
int SizeIzd(TCandle *aCandle, TMats *aMats, TPrice *aPrice, TCosts *aCosts); // расчет цены изделия


int menu(); //вывод меню на экран
void viewHelp(); //вывод справки о командах
void caption(const char*); //вывод заголовка
void horLine(); //вывод горизонтальной линии
void dataLine(TCandle, int); //вывод строки таблицы с данными
void header(); //вывод заголовков столбцов таблицы
int showList(TCandle*, int); //просмотр всех элементов массива
int notEmpty(int); //список данных не пуст


int main()
{
	setlocale(LC_CTYPE, "rus");
	// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleCP(1251);
	// установка кодовой страницы win-cp 1251 в поток вывода
	SetConsoleOutputCP(1251);
	// установка размеров окна консоли (SCR_COL_NUM символов, SCR_REC_NUM+10 строк)
	char StrForSystem[50];
	int RowNum = SCR_REC_NUM + 12;
	int ColNum = SCR_COL_NUM;
	//формирование строки системных параметров для консоли
	sprintf_s(StrForSystem, "mode con cols=%d lines=%d", ColNum, RowNum);
	//установка системных параметров консоли
	system(StrForSystem);
	system(StrForSystem);
	TCandle aCandle[MAX_REC_NUM];//массив записей
	TPrice aPrice[MAX_REC_NUM];
	TMats aMats[MAX_REC_NUM];
	TCosts aCosts[MAX_REC_NUM];
	int m, size = 0;

	while (1)
	{
		m = menu(); //отображение на экране строки меню
		switch (m) //выбор действия
		{
		case 1: //передача записей в массив из текстового файла
			size = loadFromFile(aCandle, size);
			break;
		case 2: //вставка новой записи
			size = addItem(aCandle, size);
			showList(aCandle, size);
			break;
		case 3: //вывод справочника на экран
			if (notEmpty(size)) showList(aCandle, size);
			break;
		case 4: //сохранение записей из массива в файл
			if (notEmpty(size)) size = saveToFile(aCandle, size);
			break;
		case 5:
			if (notEmpty(size)) editItem(aCandle, size);
			showList(aCandle, size);
			break;
		case 6:
			if (notEmpty(size)) size = deleteItem(aCandle, size);
			showList(aCandle, size);
			break;
		case 7:
			if (notEmpty(size)) size = findItems(aCandle, size);
			break;
		case 8:
			saveToDan(aPrice, aCosts);
			break;
		case 9:
			SizeIzd(aCandle, aMats, aPrice, aCosts);
		case 10:
			viewHelp();
			break;
		case 0:
			return 0;
		}
		system("pause");
	}


}

int menu()
{
	int v;
	system("cls");
	caption(" Б А З А   Д А Н Н Ы Х  \"И З Д Е Л И Я\"");
	for (int i = 0; i < SCR_COL_NUM - 1; i++)cout << "=";
	cout << endl;
	cout << "1[Откр] 2[Добав] 3[Просм] 4[Сохр] 5[Правка] 6[Удал] 7[Поиск] 8[Ввод расх] 9 [Стоим] 10[Справка] 0[Выход]";
	cout << endl;
	for (int i = 0; i < SCR_COL_NUM - 1; i++)cout << "=";
	cout << "\nВведите номер команды -> ";
	cin >> v;
	cout << endl;
	if (v < 0 || v>10)v = 3;
	return v;
}
int loadFromFile(TCandle *aCandle, int size)
{
	string s;
	size = 0;
	TCandle Candle;

	ifstream textFile("towns.txt");
	if (!textFile)
	{
		cout << "Ошибка открытия файла!" << endl;
		return 0;
	}
		while (!textFile.eof()) //пока не конец файла, читаем строки
		{
			if (textFile.eof()) break; //ограничитель на случай неправильного числа строк
			if (size == MAX_REC_NUM) break; //ограничитель на размер массива
			getline(textFile, s);
			strcpy_s(Candle.name, s.c_str());
			getline(textFile,s);
			stringstream ss(s);
			ss >> Candle.summ;
			getline(textFile, s);
			stringstream zz(s);
			zz >> Candle.size;
			aCandle[size] = Candle;
			size++;
		}
		showList(aCandle, size);
	textFile.close();
	cout << "Загружено записей: " << size << endl;
	return size;
}

//сохранение элементов массива в текстовый файл

int saveToFile(TCandle *aCandle, int size)
{
	TCandle Candle;
	ofstream textFile("towns.txt");
	for (int i = 0; i < size; i++)
	{
		Candle = aCandle[i];
		textFile << Candle.name<< endl;
		textFile << Candle.summ << endl;
		textFile << Candle.size;
		if (i < size - 1) textFile << endl;
	}
	textFile.close();
	showList(aCandle, size);
	cout << "Сохранено записей: " << size << endl;
	return size;
}

void putItem(TCandle *aCandle)
{
	cin.get();
	cout << "Название изделия: ";
	cin.getline(aCandle->name, 25);
	cout << "Кол-во: ";
	cin >> aCandle->summ;
	cout << "Стоимость: ";
	cin >> aCandle->size;
}

//добавление нового элемента в конец массива
int addItem(TCandle *aCandle, int size)
{
	if (size > MAX_REC_NUM - 1)
	{
		caption("Максимальное число записей превышено!");
		return size;
	}
	system("cls");
	caption("Добавление нового элемента в конец списка");
	TCandle Candle;
	cout << "Введите значения полей:" << endl;
	putItem(&Candle); //формирование новой записи
	aCandle[size] = Candle;
	return size + 1;
}

//корректировка полей выбранного элемента

int editItem(TCandle *aCandle, int size)
{
	showList(aCandle, size);
	int k;
	cout << "Введите номер записи для редактирования: ";
	cin >> k;
	system("cls");
	k--;
	caption("Редактирование полей выбранного элемента");
	horLine();
	dataLine(aCandle[k], k);
	horLine();
	TCandle Candle;
	cout << "Введите новые значения полей (\"0\" - без изменений)" << endl;
	putItem(&Candle); //формирование новой записи
	if (strcmp(Candle.name, "0") != 0) strcpy_s(aCandle[k].name, Candle.name);
	if (Candle.summ != 0) aCandle[k].summ, Candle.summ;
	if (Candle.size != 0) aCandle[k].size, Candle.size;
	return size;
}

//удаление выбранного элемента

int deleteItem(TCandle *aCandle, int size)
{
	showList(aCandle, size);
	int k;
	cout << "Введите номер записи для удаления: ";
	cin >> k;
	system("cls");
	k--;
	caption("Удаление выбранного элемента");
	horLine();
	dataLine(aCandle[k], k);
	horLine();
	int yes;
	cout << "Для подтверждения удаления элемента введите 1: ";
	cin >> yes;
	if (yes == 1) for (int i = k; i < size - 1; i++) aCandle[i] = aCandle[i + 1];
	return size - 1;
}

//Ввод данных о расходах

int saveToDan(TPrice *aPrice, TCosts *aCosts)
{
	TPrice Price;
	TCosts Costs;
	cout << "Введите цену парафина, красителя, ниток ";
	cin >> Price.paraf >> Price.kras >> Price.nit;
	cout << endl << "Введите налог на прибыль, на изделие, торговую надб: ";
	cin >> Costs.prib >> Costs.izd >> Costs.nadb;
	return 0;
}

//расчет цены изделия

int SizeIzd(TCandle *aCandle, TMats *aMats, TPrice *aPrice, TCosts *aCosts)
{
	float t;
	string a;
	TMats Mats;
	TCandle Candle;
	cin.get();
	cout << "Введите название"<<endl;
	cin.getline (Candle.name,25);
	cout << "Введите кол-во ";
	cin >> Candle.summ;
	cout << "Введите кол-во парафина ";
	cin >> Mats.paraf;
	cout << "Введите кол-во	красителя ";
	cin >> Mats.kras;
	cout << "Введите кол-во	 ниток ";
	cin	>> Mats.nit;
	cout << endl;
	printf("| %-10s| %-10s| %-10s|\n", "Цена парафина", "Красителя", "Ниток");
	printf("| %-10.2f   | %-10.2f| %10.2f|\n", Price.paraf, Price.kras, Price.nit);
	printf("| %-20s| %-20s| %-20s|\n", "Налог на прибыль", "Налог на изделие", "Торговая надбавка");
	printf("| %-20.3f| %-20.3f| %20.3f|\n", Costs.prib, Costs.izd, Costs.nadb);
	t = Price.paraf*Mats.paraf + Price.kras*Mats.kras + Price.nit*Mats.nit;
	t = (t + t * Costs.prib + t * Costs.izd + t * Costs.nadb)/Candle.summ;
	Candle.size = t;
	cout << "Цена изделия за 1 ед: " << Candle.size<<endl;
	horLine();

	return 0;
}
// поиск по выбранному полю
int findItems(TCandle *aCandle, int size)
{
	int k;
	TCandle Candle;
	cout << "Введите номер поля для поиска (1 - Название, 2 – Количество, 3 - Цена): ";
	cin >> k;
	if (k < 1 || k > 3) k = 0; //если ничего не выбрано, показываем весь список
	FindField = k;
	if (FindField != 0)
	{
		cout << "Выбрано поле " << fieldName[k] << endl;
		cout << "Введите текст (фрагмент текста) для поиска: ";
		if (k == 1) cin >> Candle.name;
		if (k == 2) cin >> Candle.summ;
		if (k == 3) cin >> Candle.size;
	}
	showList(aCandle, size);
	FindField = 0;
	return size;
}


//отображение справки о командах меню

void viewHelp()
{
	caption("Справка о командах меню");
	horLine();
	cout << " 1[Откр]    - ОТКРЫТЬ файл и загрузить данные из него" << endl;
	cout << " 2[Добав]   - ДОБАВИТЬ новую запись" << endl;
	cout << " 3[Просм]   - ПРОСМОТР всех элементов массива" << endl;
	cout << " 4[Сохр]    - СОХРАНИТЬ данные в файл" << endl;
	cout << " 5[Правка]  - ПРАВКА содержимого выбранной записи" << endl;
	cout << " 6[Удал]    - УДАЛИТЬ выбранную запись" << endl;
	cout << " 7[Поиск]    - ПОИСК записей" << endl;
	horLine();
	cout << " 8[Ввод расх]   - Ввод данных о расходах" << endl;
	horLine();
	cout << " 9[Стоим]  - Стоимость изделия при данном кол-ве и расходах" << endl;
	horLine();
	cout << "10[Справка] - СПРАВКА о командах меню" << endl;
	cout << " 0[Выход]   - ВЫХОД из программы" << endl;
	horLine();
}

//отображение строки заголовка

void caption(const char *c)
{
	for (int i = 0; i < 20; i++)cout << " "; cout << c << endl;
}

//отображение горизонтальной линии

void horLine()
{
	for (int i = 0; i < SCR_COL_NUM - 1; i++)cout << "-"; cout << endl;
}

//отображение строки таблицы с записью

void dataLine(TCandle Candle, int index)
{
	printf("| %3d | %-25s | %-10d |    %10.2f |\n", index + 1, Candle.name, Candle.summ, Candle.size);
}

//отображение заголовков столбцов таблицы

void header()
{
	printf("| %3s | %-25s | %-10s | %-10s |\n", " №", "Название изделия", "Количество", "Цена (рублей)");
}

//просмотр всех элементов массива

void showTitleForList()
{
	caption("С П И С О К   И З Д Е Л И Й");
	horLine();
	header();
	horLine();
}

int showList(TCandle *aCandle, int size)
{
	bool LineOk;
	if (FindField != 0)
	{
		cout << endl;
		horLine();
		cout << "РЕЗУЛЬТАТЫ ПОИСКА ПО ПОЛЮ \"" << fieldName[FindField] << "\"" << endl;
		horLine();
	}
	showTitleForList();
	int v;
	int LinesOnPage = SCR_REC_NUM;
	int CurPage = 1;
	int LinesCount = 0;
	for (int i = 0; i < size; i++)
	{
		LineOk = true;
		if (FindField == 1) LineOk = (strcmp(FindMan.name, aCandle[i].name) == 0);
		if (LineOk)
		{
			dataLine(aCandle[i], i);
			LinesCount++;
		}

		if (LinesCount >= LinesOnPage)
		{
			LinesCount = 0;
			cout << "для перехода к следующей странице введите 1: ";
			cin >> v;
			system("cls");
			showTitleForList();
		}
	}
	horLine();
	return size;
}

//проверка списка на количество элементов

int notEmpty(int size)
{
	if (size > 0)return 1;
	else
	{
		cout << endl << "Список пуст! (ПОДСКАЗКА: выполните команду ОТКРЫТЬ или ДОБАВИТЬ)" << endl;
		return 0;
	}
}