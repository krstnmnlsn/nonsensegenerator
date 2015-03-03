#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

int n = 3000;
int offset_int;

map<int, string> int2str;
map<string, int> str2int; // this is really just used as a set to see if we have a word or not already

vector<map<string, int> > trans(n*n, map<string, int>());

map<string, string> syns;

string get_next(int idx) {
	int totval = 0;
	for (map<string,int>::iterator it=trans[idx].begin(); it!=trans[idx].end(); ++it) {
		totval += it->second;
	}

	if (totval == 0) return ".";

	int chosenval = rand() % (totval);

	int val = 0;
	for (map<string,int>::iterator it=trans[idx].begin(); it!=trans[idx].end(); ++it) {
		val += it->second;
		if (val > chosenval) return it->first;
	}

	return " THERE HAS BEEN AN ERROR."; // this should not happen
}

string lower_case_plz(string word) {
	string new_word;
	for (int i=0; i<word.size(); ++i) {
		char c = tolower(word[i]);
		new_word.push_back(c);
	}
	return new_word;
}

string synomym(string word) {
	if (syns.count(word))
		return syns[word];
	return word;
}

void read_source(string source_file, int w) {
	ifstream source;
	source.open(source_file);
	if (source.is_open()) {
		string word;
		int prev_idx = 1; // index of last word seen as we're readinhg
		int prev_prev_idx = 1; // index of the word before that
		int next_idx = offset_int; // next number to use when you see a new word
		string punctuation = "~";
		while (source >> word) {
			word = lower_case_plz(word);
			word = synomym(word);
			if (word.back() == '>' || word[0] == '>' || word.back() == ':' || word.find('-') != string::npos ||
				word.find('#') != string::npos || word.find('\\') != string::npos || word.find('*') != string::npos) continue;
			if (word.back() == '.' || word.back() == '?' || word.back() == '\"' || word.back() == '\'' || word.back() == ',') {
				string dumb_str; dumb_str.push_back(word.back());
				punctuation = dumb_str;
				word = word.substr(0, word.size()-1);
			}
			int idx; 
			if (str2int.count(word)) 
				idx = str2int[word];
			else if (next_idx < n-1) { 
				str2int[word] = next_idx; 
				int2str[next_idx] = word; 
				idx = next_idx;
				next_idx++;
			} else {
				punctuation = "~";
				prev_idx = prev_prev_idx = 1;
				continue;
			}
			trans[prev_prev_idx*n + prev_idx][word] += w;
			prev_prev_idx = prev_idx;
			prev_idx = idx;

			if (punctuation != "~") {
				trans[prev_idx*n + idx][punctuation] += w;
				prev_prev_idx = prev_idx;
				prev_idx = str2int[punctuation];
				if (punctuation == "." || punctuation == "?") prev_idx = prev_prev_idx = 1; // go back to tilda
				punctuation = "~";
			}
		}
	}
	source.close();
}

void init_syns();

int main() {
	
	// populate transition matrix and maps (make sure "." is in there and we'll use ~ to mark the start of a new sentence)
	int2str[0] = ".";
	str2int["."] = 0;
	int2str[1] = "~";
	str2int["~"] = 1;
	int2str[2] = ",";
	str2int[","] = 2;
	int2str[3] = "?";
	str2int["?"] = 3;
	int2str[4] = "\"";
	str2int["\""] = 4;
	int2str[5] = "\'";
	str2int["\'"] = 5;
	offset_int = 6;

	init_syns();

	read_source("sourcetext2.txt", 2);
	read_source("sourcetext3.txt", 2);
	read_source("sourcetext4.txt", 1);

	// construct a sentence and output it

	while (1) {
		string dummy;
		getline(cin, dummy);
		srand(clock());
		string prev_word = "~";
		string word = "~";
		bool first = true;
		while (word != "." && word != "?") {
			string temp = word;
			word = get_next(str2int[prev_word]*n + str2int[word]);
			prev_word = temp;
			if (!first && word != "." && word != "?") cout << " ";
			first = false;
			cout << word;
		}
		cout << endl;
	}

	return 0;
}

void init_syns() {
	syns["you"] = "u";
	syns["what"] = "wut";
	syns["ill"] = "i'll";
	syns["theyre"] = "they're";
	syns["one"] = "1"; syns["two"] = "2"; syns["three"] = "3"; syns["four"] = "4"; syns["five"] = "5"; syns["six"] = "6"; syns["seven"] = "7"; syns["eight"] = "8"; syns["nine"] = "9";
	syns["okay"] = "ok";
	syns["because"] = "bc";
	syns["youve"] = "you've";
	syns["youre"] = "you're";
	syns["thats"] = "that's";
	syns["dont"] = "don't";
	syns["theyll"] = "they'll";
	syns["im"] = "i'm";
	syns["r"] = "are";
	syns["y"] = "why";
	syns["id"] = "i'd";
	syns["didnt"] = "didn't";
	syns["wouldve"] = "would've";
	syns["theres"] = "there's";
	syns["dont"] = "don't";
	syns["wouldnt"] = "wouldn't";
	syns["yur"] = "your";
	syns["ur"] = "your";
	syns["wut"] = "what";
	syns["mrs."] = "mrs";
	syns["mr."] = "mr";
	syns["hed"] = "he'd";

}