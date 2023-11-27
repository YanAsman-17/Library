#pragma once
#include<string>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h> 
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

enum class enTypeActionMenu {
	EXIT,
	LIB_BOOKS,
	USER_BOOKS,
	ERR
};

enum class enTypeBooksCatalogViewer {
	LIB,
	USER_HISTORY
};

class User
{
	int unsigned m_id;
	void checkBook(int book_id, const std::unique_ptr<sql::Connection>& conn);
	std::string getStrUserBooks(int limit = 100, int offset = 0);

	std::string getStrLibBooks(int maxCntBooksInPage, int cntBooksSkip, bool search = false, std::string searchWords = "");
	std::string getStrUserHistoryBooks(int maxCntBooksInPage, int cntBooksSkip, bool search, std::string searchWords);

	void delUserBook(int book_type_id, const std::unique_ptr<sql::Connection>& conn);
	void getElecOrAudioBook(int book_type_id, const std::unique_ptr<sql::Connection>& conn);
	void getPrintedBook(unsigned int book_type_id, const std::unique_ptr<sql::Connection>& conn);
	void booksCatalogViewer(enTypeBooksCatalogViewer, const std::unique_ptr<sql::Connection>& conn);
	void catalog(const std::unique_ptr<sql::Connection>& conn);
	void personalArea(const std::unique_ptr<sql::Connection>& conn);
public:
	User(int unsigned id = -1);
	void menu(const std::unique_ptr<sql::Connection>& conn);
};

void coutStrWithLength(std::string str, int len);
bool checkStrIsNumb(const std::string& str);
std::string searchBook(std::string searchWords, int cntBooksInPage, int offset, bool search);
