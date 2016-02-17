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

/*!
 * \brief Выводит сообщение об ошибке и закрывает программу с кодом ошибки = 1
 * \param errorText Текст ошибки
 */
void printError( char * errorText )
{
	printf( errorText );
	exit( 1 );
}

/*!
 * \brief Чтение номеров файлов, открытых процессами-предками
 * \param n Количество процессов-предков
 * \return	Массив номеров файлов
 */
int * readParentFiles( int n )
{
	printf( "Введите количество файлов, открытых %d процессами-предками:\n", n );
	int * mas = malloc( sizeof(int) * n );
	for ( int i = 0; i < n; ++i ) {
		scanf( "%d", &mas[i] );
		if ( mas[i] < 0 )
			printError( "Количество открытых файлов должно быть неотрицательным числом.\n" );
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
	printf( "Введите количество файлов, открытых %d процессами-потомками:\n", n );
	int * mas = malloc( sizeof(int) * n );
	for ( int i = 0; i < n; ++i ) {
		scanf( "%d", &mas[i] );
		if ( mas[i] < 0 ) {
			printError( "Количество открытых файлов должно быть неотрицательным числом.\n" );
		}
	}

	return mas;
}

/*!
 * \brief Вывод таблицы открытых файлов
 * \param count Количество открытых файлов
 */
void printTable( int count )
{
	int n = count;

	char filenameStr[]	 = "Имя файла";
	char descriptorStr[] = "Дескриптор";

	const int FIELD_SIZE = 30;

	printf( "%-*s", FIELD_SIZE, filenameStr );
	printf( "%s\n", descriptorStr );

	char stdinStr[]	 = "stdin";
	char stdoutStr[] = "stdout";
	char stderrStr[] = "stderr";

	printf( "%-*s", FIELD_SIZE, stdinStr ); printf( "0\n" );
	printf( "%-*s", FIELD_SIZE, stdoutStr ); printf( "1\n" );
	printf( "%-*s", FIELD_SIZE, stderrStr ); printf( "2\n" );

	for ( int j = 0; j < n; ++j ) {
		printf( "Файл %-*d", FIELD_SIZE-5, j ); printf( "fd%d\n", j );
	}
}

/*!
 * \brief Вывод таблицы открытых файлов процессов-предков с шапкой таблицы
 * \param Номер процесса
 * \param Количество файлов, открытых i-ым процессом
 */
void printFileTableParent( int i, int count )
{
	printf( "Таблица файлов %d процесса-предка:\n", i );
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
	printFileTableParent( i, count );
}

/*!
 * \brief Вывод таблицы открытых файлов процессов-предков с шапкой таблицы
 * \param Номер процесса
 * \param Количество файлов, открытых i-ым процессом
 */
void printFileTableChildren( int i, int parentCount, int ownAdd )
{
	printf( "Таблица файлов %d процесса-потомка:\n", i );
	printTable( parentCount + ownAdd );
}

int main()
{
	setlocale( LC_ALL, "Rus" );

	//! Чтение количества процессов-предков
	int n;
	printf( "Задайте количество процессов-предков:\n" );
	scanf( "%d", &n );
	if ( n < 1 )
		printError( "Количество процессов-предков должно быть положительным целым числом.\n" );

	//! Чтение количества файлов, открытых процессами-предками
	int * files = readParentFiles( n );
	for ( int i = 0; i < n; ++i )
		printFileTableParent( i, files[i] );

	//! Чтение количества процессов-потомков
	int m;
	printf( "Задайте количество процессов-потомков:\n" );
	scanf( "%d", &m );
	if ( m < 0 )
		printError( "Количество процессов-потомков должно быть неотрицательным целым числом.\n" );

	int * parentId = malloc( sizeof(int) * m );
	for ( int i = 0; i < m; ++i ) {
		printf( "Задайте родитель для %d потомка, %d процесса: ", i, i + n );
		scanf( "%d", &parentId[i] );
		printf("\n");
	}

	//! Вывод таблиц файлов для процессов-потомков
	for ( int i = 0; i < m; ++i )
		printChildrenBaseTable( i+n, files[ parentId[i] ] );

	//! Чтение количества процессов-потомков, что открыли доп файлы.
	int k;
	printf( "Задайте количество процессов-потомков, которые открыли дополнительные файлы:\n" );
	scanf( "%d", &k );
	if ( k < 0 )
		printError( "Количество процессов-потомков должно быть неотрицательным целым числом.\n" );

	//! Чтение количества файлов, открытых k потомками
	int * childrenFiles = readChildrenFiles( k );
	for ( int i = 0; i < k; ++i )
		printFileTableChildren( i+n, files[i], childrenFiles[i] );

	return 0;
}
