/*!
 * \brief	Данная программа может быть использована для демонстрации
 *			создания процессов, их потомков (системным вызовом fork()), а также таблиц открытых файлов этих процессов.
 *
 * \author	Рогоза А. А.
 * \author	Романов С. А.
 * \date	17/02/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include <iostream>
using namespace std;

/*!
 * \brief Выводит сообщение об ошибке и закрывает программу с кодом ошибки = 1
 * \param errorText Текст ошибки
 */
void printError( const char * errorText )
{
	cout << errorText << endl;
	exit( 1 );
}

/*!
 * \brief Чтение номеров файлов, открытых процессами-предками
 * \param n Количество процессов-предков
 * \return	Массив номеров файлов
 */
int * readParentFiles( int n )
{
	cout << "Введите количество файлов, открытых " << n << " процессами-предками:" << endl;
	int * mas = new int[n];
	for ( int i = 0; i < n; ++i ) {
		cin >> mas[i];
		if ( mas[i] < 0 )
			printError( "Количество открытых файлов должно быть неотрицательным числом." );
	}

	return mas;
}

/*!
 * \brief Чтение номеров файлов, открытых процессами-потомками
 * \param n Количество процессов-потомков, что открыли доп. файлы
 * \return	Массив номеров файлов
 *
 * \details TODO_2:
 */
int * readChildrenFiles( int n )
{
	cout << "Введите количество файлов, открытых " << n << " процессами-потомками:" << endl;
	int * mas = new int[n];
	for ( int i = 0; i < n; ++i ) {
		cin >> mas[i];
		if ( mas[i] < 0 )
			printError( "Количество открытых файлов должно быть неотрицательным числом." );
	}

	return mas;
}

/*!
 * \brief Вывод таблицы открытых файлов
 * \param count Количество открытых файлов
 */
void printTable( int count )
{
	const int FIELD_WIDTH = 30;

	cout.width( FIELD_WIDTH );
	cout << std::left;
	cout << "Имя файла";
	cout << "Дескриптор" << endl;

	cout.width( FIELD_WIDTH );
	cout << "stdin";
	cout << 0 << endl;

	cout.width( FIELD_WIDTH );
	cout << "stdout";
	cout << 1 << endl;

	cout.width( FIELD_WIDTH );
	cout << "stderr";
	cout << 2 << endl;

	for ( int j = 0; j < count; ++j ) {
		cout.width( FIELD_WIDTH + 4 );
		cout << "Файл " + std::to_string( j );
		cout << "fd" + std::to_string( j ) << endl;
	}
}

/*!
 * \brief Вывод таблицы открытых файлов процессов-предков с шапкой таблицы
 * \param Номер процесса
 * \param Количество файлов, открытых i-ым процессом
 */
void printFileTableParent( int i, int count )
{
	cout << "Таблица файлов " << i << " процесса-предка:" << endl;
	printTable( count );
}

/*!
 * \brief Вывод таблицы открытых файлов процессов-потомков с шапкой таблицы
 *
 * \param Номер процесса
 * \param Количество файлов, открытых i-ым процессом
 *
 * \details Таблица в тот момент, когда только был порожден процесс-потомок, т.е. полная копия таблицы открытых файлов процесса-предка.
 */
void printChildrenBaseTable( int i, int count )
{
	cout << "Таблица файлов " << i << " процесса-потомка:" << endl;
	printTable( count );
}

/*!
 * \brief Вывод таблицы открытых файлов процессов-предков с шапкой таблицы
 * \param Номер процесса
 * \param Количество файлов, открытых i-ым процессом
 */
void printFileTableChildren( int i, int parentCount, int ownAdd )
{
	cout << "Таблица файлов " << i << " процесса-потомка:" << endl;
	printTable( parentCount + ownAdd );
}

int main()
{
	setlocale( LC_ALL, "Rus" );

	//! Чтение количества процессов-предков
	int n;
	cout << "Задайте количество процессов-предков:" << endl;
	cin >> n;
		if ( n < 1 )
		printError( "Количество процессов-предков должно быть положительным целым числом.\n" );

	//! Чтение количества файлов, открытых процессами-предками
	int * files = readParentFiles( n );
	for ( int i = 0; i < n; ++i )
		printFileTableParent( i, files[i] );

	//! Чтение количества процессов-потомков
	int m;
	cout << "Задайте количество процессов-потомков:" << endl;
	cin >> m;
	if ( m < 0 )
		printError( "Количество процессов-потомков должно быть неотрицательным целым числом." );

	if ( m > n )
		printError( "Количество процессов-потомков не может быть больше, чем количество процессов-предков." );

	int * parentId = new int[m];
	for ( int i = 0; i < m; ++i ) {
		cout << "Задайте родитель для " << i << " потомка, " << i+n << " процесса: ";
		cin >> parentId[i];
		if ( parentId[i] >= n )
			printError( "Номер предка не может превышать количества процессов-предков." );
		cout << endl;
	}

	//! Вывод таблиц файлов для процессов-потомков
	for ( int i = 0; i < m; ++i )
		printChildrenBaseTable( i, files[ parentId[i] ] );
	delete parentId;

	//! Чтение количества процессов-потомков, что открыли доп файлы.
	int k;
	cout << "Задайте количество процессов-потомков, которые открыли дополнительные файлы:" << endl;
	cin >> k;
	if ( k < 0 )
		printError( "Количество процессов-потомков должно быть неотрицательным целым числом." );

	if ( k > m )
		printError( "Количество процессов-потомков, что дополнительно открывают файлы, не должно превышать количество процессов-потомков" );

	//! Чтение номер процессов-потомков, что открывают файлы
	int * childrenId = new int[k];
	for ( int i = 0; i < k; ++i ) {
		cout << endl;
		cout << "Задайте номер процесса-потомка, что открывает дополнительные файлы: ";
		cin >> childrenId[i];
		if ( childrenId[i] >= m )
			printError( "номер процесса-потомка для процесса, открывающего дополнительные файлы,  не может превышать количества процессов-потомков-1." );
	}
	cout << endl;

	//! Чтение количества файлов, открытых k потомками
	int * childrenFiles = readChildrenFiles( k );
	for ( int i = 0; i < k; ++i )
		printFileTableChildren( childrenId[i], files[ childrenId[i] ], childrenFiles[i] );

	return 0;
}
