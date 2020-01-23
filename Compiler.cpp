#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>

using namespace std; //use standard library

// ******************************************************
// *** PART 1 FUNCTIONS *********************************
// ******************************************************

void removeComments(vector<char> &currVect, vector<char> &newVect, int size); //remove comment areas from given code
void removeBlankLines(vector<char> &currVect, vector<char> &newVect, int size); //remove lines that are entirely white space from given code
void removeExtraSpaces(vector<char> &currVect, vector<char> &newVect, int size); //remove excess spaces within the given code
void addSpaces(vector<char> &currVect, vector<char> &newVect, int size); //add spaces to the given code where needed
void newToCurr(vector<char> &currVect, vector<char> &newVect, int size); //copy given code from one vector to another and cleat the vector that was copied from

// ******************************************************
// *** PART 2 FUNCTIONS AND GLOBAL VARIABLES ************
// ******************************************************

const string readFilename = "pre-compiled_code.txt";
vector<string> variables;

bool keyword(string keyword, string line);
bool id(string token);
bool dec_list(string line, int lineNum);
bool dec(string line, int lineNum);
bool stat_list(queue<string> *line, int *lineNum);
bool stat(string line, int lineNum);
bool print(string line, int lineNum);
bool output(string line, int lineNum);
bool assign(string line, int lineNum);
bool expr(string line, int lineNum);
bool term(string line, int lineNum);
bool factor(string line, int lineNum);
bool number(string line);
bool type(string line, int lineNum);
bool digit(char c);
bool letter(char c);
bool varExists(string var, int lineNum);


//Node struct for INTEGER declaration
struct Node {
	string name;
	int data;
	Node* next;
};

Node* vars = new Node();

int main() {

	// ******************************************************
	// *** PART 1 MAIN **************************************
	// ******************************************************

	ifstream givenCode;
	ofstream newCode;
	char temp2; //used to temporarily contain characters to put into a vector
	vector<char> currVect, newVect; //vectors used to contain the given and new code character by character

	givenCode.open("final.txt"); //open given file with code in it

	if (!givenCode.is_open()) { //checks if the file opened properly, if not, end program
		cout << "Error. File did not open." << endl;
		return 0;
	}

	while (givenCode.get(temp2)) { //reads in the given code character by character
		currVect.push_back(temp2); //puts the read in characters into a vector of characters.
	}

	givenCode.close(); //close the given file; no longer necessary

	removeComments(currVect, newVect, currVect.size()); //remove comments from given code
	newToCurr(currVect, newVect, newVect.size()); //replace old code vector with newly created code vector

	removeExtraSpaces(currVect, newVect, currVect.size()); //remove extra spaces from current code vector
	newToCurr(currVect, newVect, newVect.size()); //replace old code vector with newly created code vector

	removeBlankLines(currVect, newVect, currVect.size());//remove blank lines from current code vector
	newToCurr(currVect, newVect, newVect.size());//replace old code vector with newly created code vector

	addSpaces(currVect, newVect, currVect.size()); // add necessary spaces to current code vector
	newToCurr(currVect, newVect, newVect.size()); //replace old code vector with newly created code vector

	newCode.open("pre-compiled_code.txt"); //open new file to write re-formatted code to

	if (!newCode.is_open()) { //checks if the file opened properly, if not, end program
		cout << "Error. File did not open." << endl;
		return 0;
	}

	for (size_t i = 0; i < currVect.size(); i++) { //write code out to file
		newCode << currVect[i];
	}

	newCode.close(); // close the file with the re-formatted code

					 // ******************************************************
					 // *** PART 2 MAIN **************************************
					 // ******************************************************

	ifstream read(readFilename);
	string projectPname, endS, temp;
	queue<string> line, varDecList, beginStatList;
	bool var = true, begin = false, end = false, accepted = true;

	int lineNum = 1;
	getline(read, projectPname);
	if (!keyword("PROGRAM", projectPname)) {
		cout << "PROGRAM is expected		line #" << lineNum << endl;
		accepted = false;
	}
	if (projectPname[projectPname.size() - 1] == ';') projectPname = projectPname.substr(0, projectPname.size() - 1);
	else {
		cout << "; is missing		line #" << lineNum << endl;
		accepted = false;
	}
	if (!id(projectPname.substr(8, projectPname.size() - 8))) {
		cout << "PROGRAM name expected as an identifier			line #" << lineNum << endl;
		accepted = false;
	}

	while (getline(read, temp))
	{
		if (keyword("BEGIN", temp)) {
			begin = true;
			var = false;
		}
		if (temp.find("END") == 0 && (temp.size() == 3 || temp[temp.size() - 1] == '.' || temp[temp.size() - 1] == ' ')) {
			end = true;
			begin = false;
			var = false;
		}

		if (var)varDecList.push(temp);
		else if (begin)	beginStatList.push(temp);

		else if (end) {
			endS = temp;
			end = false;
			while (getline(read, temp));
		}
	}

	temp = varDecList.front();
	varDecList.pop();
	lineNum++;
	if (!keyword("VAR", temp)) {
		cout << "VAR is expected" << endl;
		accepted = false;
	}

	if (varDecList.size() != 0) {
		temp = varDecList.front();
		varDecList.pop();
		lineNum++;
		if (temp[temp.size() - 1] != ';') {
			cout << "; is missing			line #" << lineNum << endl;
			accepted = false;
		}
		else temp = temp.substr(0, temp.size() - 1);
		if (!dec_list(temp, lineNum)) accepted = false;
	}

	if (beginStatList.size() == 0) {
		cout << "BEGIN is expected" << endl;
		accepted = false;
		if (varDecList.size() > 0) stat_list(&varDecList, &lineNum);
	}

	else {
		temp = beginStatList.front();
		beginStatList.pop();
		lineNum++;
		if (!stat_list(&beginStatList, &lineNum)) accepted = false;
		lineNum++;
	}

	if (endS.size() == 0 || !keyword("END.", endS)) {
		if (endS.find("END") == 0)
			cout << ". is missing			line #" << lineNum << endl;
		cout << "END. is expected" << endl;
		accepted = false;
	}

	// ******************************************************
	// *** PART 3 MAIN **************************************
	// ******************************************************

	if (accepted) {
		cout << "ACCEPTED" << endl;
		read.close();
		read.open(readFilename);
		string line, token;
		Node* temp = vars;
		getline(read, line);
		string cppName = line.substr(8, line.size() - 9) + ".cpp";
		ofstream in(cppName);
		in << "#include <iostream>\n"
			<< "using namespace std;\n";

		while (getline(read, line)) {
			if (line.compare("BEGIN") == 0) in << "int main()\n"
				<< "{\n";
			else if (line.size() > 9 && line.substr(line.size() - 8, 7).compare("INTEGER") == 0) {
				token = line.substr(0, line.find(' '));
				in << "int ";
				while (line[0] != ':') {
					line = line.substr(token.size() + 1, line.size() - token.size());
					if (token[token.size() - 1] == ',') token = token.substr(0, token.size() - 1);
					temp->name = token;
					temp->next = new Node();
					temp = temp->next;
					in << token;
					if (line[0] == ':') in << ";\n";
					else in << ", ";
					token = line.substr(0, line.find(' '));
				}
				temp = vars;
			}

			else if (line.find("PRINT") == 0) {
				in << "cout << ";
				int pos1 = line.find('(') + 1;
				int pos2 = line.find(')');
				line = line.substr(pos1, pos2 - pos1);
				pos1 = line.find('\'');
				while (pos1 != string::npos) {
					line = line.substr(pos1 + 1, line.size() - pos1 - 1);
					pos2 = line.find('\'');
					in << "\"" << line.substr(0, pos2) << "\" <<";
					line = line.substr(pos2 + 3, line.size() - 3 - pos2);
					pos1 = line.find('\'');
				}
				pos1 = line.find(' ');
				if (pos1 == 0) {
					line = line.substr(1, line.size() - 1);
					pos1 = line.find(' ');
				}
				if (pos1 == line.size() - 1) line = line.substr(0, line.size() - 1);
				pos1 = line.find(' ');
				while (pos1 != string::npos) {
					if (pos1 == line.size() - 1) break;
					token = line.substr(1, pos1 - 3);
					in << "\"" << token << "\" << ";
					line = line.substr(pos1 + 1, line.size() - pos1 - 1);
					pos1 = line.find(' ');
				}
				token = line;
				if (token[token.size() - 1] == ' ') token = token.substr(0, token.size() - 1);
				while (temp->name.compare(token) != 0) temp = temp->next;
				in << temp->name << ";\n";
				temp = vars;
			}
			else if (line.compare("END.") == 0) in << "return 0;\n}";
			else if (line.compare("VAR") != 0) in << line << "\n";
		}

		in.close();
	}
	else cout << "NOT ACCEPTED" << endl;
	cin.get();
	return 0;
}

// ******************************************************
// *** PART 1 FUNCTIONS**********************************
// ******************************************************

void removeComments(vector<char> &currVect, vector<char> &newVect, int size) { //remove comments from current vector code and put new code into new vector
	char curr, prev, next;
	bool comment = false;

	for (int i = 0; i < size; i++) {
		if (i == 0)
			prev = NULL; //previous is null if current is the first value
		else
			prev = currVect[i - 1];

		if (i == size - 1)
			next = NULL; //next is null if current is the last value
		else
			next = currVect[i + 1];

		curr = currVect[i];

		if (curr == '(' && next == '*') { //checks if a comment is beginning
			comment = true;
		}

		if (!comment) { //if not in a comment, add char to new vector
			newVect.push_back(curr);
		}

		if (curr == '*' && next == ')') { //checks for end of a comment
			comment = false;
			i = i + 1; //skips ahead one space to not include ')' in the new code
		}

	}
}

void removeBlankLines(vector<char> &currVect, vector<char> &newVect, int size) { //remove blank likes from current code and put new code into new vector
	char curr, prev, next;
	bool skippable = false;

	for (int i = 0; i < size; i++) {
		if (i == 0)
			prev = NULL; //previous is null if current is the first value
		else
			prev = currVect[i - 1];

		if (i == size - 1)
			next = NULL; //next is null if current is the last value
		else
			next = currVect[i + 1];

		curr = currVect[i];

		if ((curr == '\n' && prev == NULL) || (curr == '\n' && prev == '\n')) { //checks if there is no content between new lines

		}
		else {
			newVect.push_back(curr);
		}

	}
}

void removeExtraSpaces(vector<char> &currVect, vector<char> &newVect, int size) { //Removes excess spaces where they are not needed in the current code vector and puts new code into new vector
	char curr, prev, next;
	bool skippable = false;
	bool spaceAtFront = false;
	bool spaceBeforeSemi = false;

	for (int i = 0; i < size; i++) {
		if (i == 0)
			prev = NULL; //previous is null if current is the first value
		else
			prev = currVect[i - 1];

		if (i == size - 1)
			next = NULL; //next is null if current is the last value
		else
			next = currVect[i + 1];

		curr = currVect[i];

		if ((i == 0 || prev == '\n') && isspace(curr)) { //checks if there are spaces at the beginning of code
			spaceAtFront = true;
		}
		if (spaceAtFront) {
			if (!isspace(curr)) {
				spaceAtFront = false;
			}
		}
		if (!spaceAtFront) {

			if (isspace(curr) && (next == ';' || next == ',')) { //checks for spaces before commas and semicolons
				spaceBeforeSemi = true;
			}
			if (spaceBeforeSemi) {
				if ((curr == ';' || curr == ',')) {
					spaceBeforeSemi = false;
				}
			}
			if (!spaceBeforeSemi) {
				if (isspace(curr) && isspace(next)) { //checks for double spaces
					skippable = true;
				}

				if (skippable) {
					if (curr == '\n' || !isspace(curr) || !isspace(next)) //checks for characters that are able to be entered into new code
						skippable = false;
				}

				if (!skippable) {
					newVect.push_back(curr);
				}
			}
		}
	}
}

void addSpaces(vector<char> &currVect, vector<char> &newVect, int size) { //adds spaces to locations that need it and puts them into the new code vector
	char curr, prev, next;
	bool inDQuotes = false;
	bool inSQuotes = false;

	for (int i = 0; i < size; i++) {
		if (i == 0)
			prev = NULL; //previous is null if current is the first value
		else
			prev = currVect[i - 1];

		if (i == size - 1)
			next = NULL; //next is null if current is the last value
		else
			next = currVect[i + 1];

		curr = currVect[i];


		if (curr == '"' && !inDQuotes) { //checks for the start of double quotes
			inDQuotes = true;
		}
		else if (curr == '"' && inDQuotes) { //checks for the end of double quotes
			inDQuotes = false;
		}

		if (curr == '\'' && !inSQuotes) { //checks for start of single quotes
			inSQuotes = true;
		}

		else if (curr == '\'' && inSQuotes) { //checks for end of single quotes
			inSQuotes = false;
		}

		if (curr == ';' && prev == ')' && next == '\n') inSQuotes = false;

		if ((next == '=' || next == '*' || next == '/' || next == '+' || next == '-' || next == ':') && !isspace(curr) && !inDQuotes && !inSQuotes) { //checks for characters that need a space before it
			newVect.push_back(curr);
			newVect.push_back(' ');
		}
		else if ((curr == '=' || curr == '*' || curr == '/' || curr == '+' || curr == '-' || curr == ',') && !isspace(next) && !inDQuotes && !inSQuotes) { //checks for characters that need a space after it
			newVect.push_back(curr);
			newVect.push_back(' ');
		}
		else
			newVect.push_back(curr);
	}
}

void newToCurr(vector<char> &currVect, vector<char> &newVect, int size) {
	currVect.clear();
	for (int i = 0; i < size; i++) { //copies contents of newVect into currVect
		currVect.push_back(newVect[i]);
	}
	newVect.clear();
}

// ******************************************************
// *** PART 2 FUNCTIONS *********************************
// ******************************************************

bool keyword(string keyword, string line)
{
	size_t length = keyword.length();
	if (line.length() < length) return false;
	int pos = line.find(keyword);
	if (pos == string::npos || pos != 0) return false;

	return line.length() == length || line[length] == ' ';
}

bool id(string token) {
	if (token.length() == 1) return isalpha(token[0]);
	return (letter(token[token.length() - 1]) || digit(token[token.length() - 1])) && id(token.substr(0, token.length() - 1));
}

bool dec_list(string line, int lineNum) {
	int pos = line.find(':');
	if (pos == string::npos) {
		cout << ": is missing" << endl;
		pos = line.find("INTEGER");
		return dec(line.substr(0, pos - 1), lineNum) && type(line.substr(pos, line.size() - pos), lineNum) && false;
	}
	return dec(line.substr(0, pos - 1), lineNum) && type(line.substr(pos + 2, line.size() - pos - 2), lineNum);
}

bool dec(string line, int lineNum) {
	int pos = line.find(' ');
	string token;
	if (pos != string::npos) {
		token = line.substr(0, pos);
		if (token[token.size() - 1] != ',') cout << ", is missing			line #" << lineNum << endl;
		else token = token.substr(0, token.size() - 1);
		if (!id(token)) cout << "Incorrect Identifier " << token << "		line #" << lineNum << endl;
		else variables.push_back(token);
		return id(token) && dec(line.substr(pos + 1, line.size() - 1 - pos), lineNum);
	}
	token = line;
	variables.push_back(token);
	return id(token);
}

bool stat_list(queue<string> *statList, int *lineNum) {
	string line = statList->front();
	statList->pop();
	(*lineNum)++;
	bool rValue = stat(line, *lineNum);
	if (statList->size() > 0)
		return stat_list(statList, lineNum) && rValue;
	return rValue;
}

bool stat(string line, int lineNum) {
	if (line[line.size() - 1] != ';') cout << "; is missing			line #" << lineNum << endl;
	else line = line.substr(0, line.size() - 1);
	if (line.find("PRINT") != string::npos) return print(line, lineNum);
	return assign(line, lineNum);
}

bool print(string line, int lineNum) {
	int pos1 = line.find('(');
	int pos2 = line.find(')');
	if (pos1 == string::npos) cout << "( is missing			line #" << lineNum << endl;
	else line = line.substr(pos1 + 1, line.size() - pos1 - 1);
	if (pos2 == string::npos) cout << ") is missing			line #" << lineNum << endl;
	else line = line.substr(0, pos2 - pos1 - 1);
	return output(line, lineNum);
}

bool output(string line, int lineNum) {
	int pos = line.find(' ');
	string token;
	if (pos == 0) {
		line = line.substr(1, line.size() - 1);
		pos = line.find(' ');
		token = line;
	}
	else if (line.find('\'') != string::npos) {
		int pos1 = line.find('\'');
		if (pos1 != 0) {
			cout << "\' is missing			line #" << lineNum << endl;
			return output(line.substr(pos1 + 1, line.size() - 1 - pos1), lineNum);
		}
		line = line.substr(1, line.size() - 1);
		pos1 = line.find('\'');
		if (pos1 == string::npos) {
			cout << "\' is missing			line #" << lineNum << endl;
			return false;
		}
		if (line[pos1 + 1] == ',') return output(line.substr(pos1 + 2, line.size() - 2 - pos1), lineNum);
		cout << ", is missing			line #" << lineNum << endl;
		return false;
	}

	else if (line.find_last_of(' ') != pos) {
		cout << "\' is missing			line #" << lineNum << endl;
		return false;
	}
	else token = line.substr(0, pos);
	if (pos == line.size() - 1 && token.at(token.size() - 1) == ' ') {
		pos = string::npos;
		token = token.substr(0, token.size() - 1);
	}
	if (pos != string::npos && pos != line.size() - 1 && token[token.size() - 1] != ',') cout << ", is missing			line #" << lineNum << endl;
	else if (token[token.size() - 1] == ',') token = token.substr(0, token.size() - 1);
	else return id(token) && varExists(token, lineNum);
	/*if (token[0] != '\'' || token[token.size() - 1] != '\'') {
	cout << "\' is missing			line #" << lineNum << endl;
	return output(line.substr(pos + 1, line.size() - pos - 1), lineNum) && false;
	}*/
	return output(line.substr(pos + 1, line.size() - pos - 1), lineNum);
}

bool assign(string line, int lineNum) {
	int pos = line.find('=');
	if (pos == string::npos) {
		if (line.find(' ') != string::npos)
			cout << "= is missing			line #" << lineNum << endl;
		pos = line.find(' ');
		if (!id(line.substr(0, pos))) cout << "Invalid Identifier " << line.substr(0, pos) << "			line #" << lineNum << endl;
		return expr(line, lineNum) && false;
	}
	if (pos < 2) {
		cout << "Missing identifier to assign to		line #" << lineNum << endl;
		return expr(line.substr(pos + 2, line.size() - pos - 2), lineNum) && false;
	}
	return id(line.substr(0, pos - 1)) && varExists(line.substr(0, pos - 1), lineNum) && expr(line.substr(pos + 2, line.size() - pos - 2), lineNum);
}

bool expr(string line, int lineNum) {
	int pos = line.find_last_of('+');
	if (pos == string::npos) pos = line.find_last_of('-');
	if (pos == string::npos) return term(line, lineNum);
	bool lastTerm = term(line.substr(pos + 2, line.size() - 2 - pos), lineNum);
	return expr(line.substr(0, pos - 1), lineNum) && lastTerm;
}

bool term(string line, int lineNum) {
	int pos = line.find_last_of('*');
	if (pos == string::npos) pos = line.find_last_of('/');
	if (pos == string::npos) return factor(line, lineNum);
	bool lastFactor = factor(line.substr(pos + 2, line.size() - 2 - pos), lineNum);
	return term(line.substr(0, pos - 1), lineNum) && lastFactor;
}

bool factor(string line, int lineNum) {
	if (line.find('+') == string::npos && line.find('-') == string::npos &&
		line.find('*') == string::npos && line.find('/') == string::npos)
	{
		if (!(id(line) && varExists(line, lineNum)) && !number(line))	return false;
		return true;
	}
	return expr(line, lineNum);
}

bool number(string line) {
	if (line.size() < 2) return digit(line[0]);
	return digit(line[0]) && number(line.substr(1, line.size() - 1));
}

bool type(string line, int lineNum) {
	if (line.compare("INTEGER") != 0) {
		cout << "INTEGER is expected		line #" << lineNum << endl;;
		return false;
	}
	return true;
}

bool digit(char c) {
	return isdigit(c);
}

bool letter(char c) {
	return isalpha(c);
}

void split(queue<string> *tokens, string line, char c)
{
	int pos = line.find_first_of(c);
	while (pos != string::npos) {
		tokens->push(line.substr(0, pos));
		line = line.substr(pos + 1, line.size() - pos + 1);
		pos = line.find_first_of(c);
	}
	tokens->push(line);
}

bool varExists(string var, int lineNum) {
	for (size_t i = 0; i < variables.size(); i++)
	{
		if (variables.at(i).compare(var) == 0)
			return true;
	}
	cout << "UNKOWN IDENTIFIER " << var << "		line #" << lineNum << endl;
	return false;
}

// ******************************************************
// *** PART 3 FUNCTIONS *********************************
// ***********************