
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include <utility> 
#define TEST_INSERT
#define TEST_REMOVE
#define TEST_GET_VALUE
#define TEST_FIND
#define TEST_SUGGESTIONS


const int ALPHABET_SIZE = 26;
struct Node
{
	int counter;
	std::string Word;
	Node* children[ALPHABET_SIZE];
	Node()
	{
		Word = "";
		counter = 0;
		for (int i = 0; i < ALPHABET_SIZE; i++)
			children[i] = NULL;
	}
};

class Trie
{
public:
	Trie()
	{
		root = new Node();
	}
	void addWord(const std::string& word)
	{
		for (int i = 0; i < word.length(); i++)
		{
			if (word[i]>'z' || word[i] < 'a')
				throw"The Trie can not recognize letters other than lowercase letters";
		}
		insertWord(word, 0, root);
	}
	bool find(const std::string& word)
	{
		Node*p = root;

		for (int i = 0; i < word.length(); i++)
		{
			int index = word[i] - 'a';

			if (!p->children[index])
				return false;

			p = p->children[index];
		}

		return (p != NULL && p->Word != "");
	}


	std::vector<std::string> getSuggestions(const std::string& beginning)
	{
		std::vector<std::string> result;

		suggest(beginning, 0, root, result);

		for (int i = 0; i < result.size(); i++)
			std::cout << result[i] << std::endl;

		return result;
	}

	bool remove(const std::string& word)
	{
		if (!find(word))
			return false;

		removing(word, 0, root);

		return true;
	}

	void print()
	{
		printAll(root);
	}

	int getValue(const std::string& word)
	{
		if (!find(word))
			return 0;

		return Value(word, 0, root);
	}


private:
	Node* root;
	void removing(const std::string& word, int pos, Node* _root)
	{
		if (word.length() == pos)
		{
			_root->Word = "";
			return;
		}
		if (_root->children[word[pos] - 'a']->counter >1)
		{
			_root->children[word[pos] - 'a']->counter--;
			removing(word, pos + 1, _root->children[word[pos] - 'a']);
		}
		else
		{
			destroy(_root->children[word[pos] - 'a']);
			_root->children[word[pos] - 'a'] = NULL;
		}
	}
	void destroy(Node *currentNode)
	{
		if (currentNode == NULL)
			return;

		for (int i = 0; i < 26; i++)
		{
			destroy(currentNode->children[i]);
		}

		delete currentNode;

	}
	void printAll(Node * _root)
	{
		for (int i = 0; i < ALPHABET_SIZE; i++)
		if (_root->children[i] != NULL)
		{
			printAll(_root->children[i]);
		}

		if (_root->Word != "")
		{
			std::cout << _root->Word << " -> " << _root->counter << std::endl;
		}

	}

	void insertWord(std::string word, int pos, Node * _root)
	{
		if (word.length() == pos)
		{
			_root->Word = word;
			return;
		}

		if (_root->children[word[pos] - 'a'] == NULL)
		{
			Node *p = new Node;
			p->counter++;
			_root->children[word[pos] - 'a'] = p;

			insertWord(word, pos + 1, _root->children[word[pos] - 'a']);
		}
		else
		{
			_root->children[word[pos] - 'a']->counter++;
			insertWord(word, pos + 1, _root->children[word[pos] - 'a']);
		}

	}

	void suggest(const std::string& beginning, int pos, Node * _root, std::vector<std::string>& result)
	{
		if ((beginning != _root->Word) && (_root->children[beginning[pos] - 'a'] != NULL))
		{
			suggest(beginning, pos + 1, _root->children[beginning[pos] - 'a'], result);
		}
		else
		{
			returnVectorWithAllWord(_root, result);
		}
	}

	void returnVectorWithAllWord(Node * _root, std::vector<std::string>& result)
	{
		for (int i = 0; i < ALPHABET_SIZE; i++)
		if (_root->children[i] != NULL)
		{
			returnVectorWithAllWord(_root->children[i], result);
		}

		if (_root->Word != "")
		{
			result.push_back(_root->Word);
		}
	}

	int Value(const std::string& word, int pos, Node * _root)
	{
		if ((word != _root->Word) && (_root->children[word[pos] - 'a'] != NULL))
		{

			Value(word, pos + 1, _root->children[word[pos] - 'a']);
		}
		else
		{
			return _root->counter;
		}
	}

};

void readAndPrint()
{
	Trie t;
	std::string word, buffer;
	char c;
	getline(std::cin, buffer);
	std::cin.clear();

	for (int i = 0; i < buffer.length(); i++)
	{
		if (buffer[i] != ' ')
		{
			word += buffer[i];
		}
		else
		{
			if (word.length() != 0)
				t.addWord(word);
			word.clear();
		}

	}
	if (word.length() != 0)
	{
		t.addWord(word);
		word.clear();
	}

	t.print();
	t.tenWord();
}


int _tmain(int argc, _TCHAR* argv[])
{

	readAndPrint();

	#ifdef TEST_INSERT
	std::cout << "\tTEST INSERT" << std::endl;
	Trie t;
	t.addWord("fmi");
	t.addWord("grades");
	t.addWord("memory");
	t.addWord("hell");
	t.addWord("history");
	t.addWord("hi");
	t.addWord("fmi");
	t.addWord("grades");
	t.addWord("high");
	std::cout << "expected :" << std::endl << "fmi - > 2 \ngrades - > 2 \nhell - > 1 \nhight - > 1 \nhistory - > 1  \nhi - > 1 \nmemory - > 1 \n" << std::endl;

	std::cout << "got :" << std::endl;
	t.print();

	#endif

	#ifdef TEST_SUGGESTIONS
	std::cout << "\n\tTEST SUGGESTIONS" << std::endl;
	std::cout << "\nSuggestions for  'hi'" << std::endl;
	std::cout << "expected :" << std::endl << "hight \nhistory \nhi" << std::endl;
	std::cout << "got :" << std::endl;
	std::vector<std::string> temp = t.getSuggestions("hi");
	#endif
	#ifdef TEST_REMOVE
	std::cout << "\n\tTEST REMOVE" << std::endl;
	std::cout << "\nRemove a word that is not in the tree is ";
	if (!t.remove("ivan"))
	std::cout << " successfully implemented\n";
	else
	std::cout << "not successfully implemented\n";
	std::cout << "\nRemove a word that is in the tree!!\n\n";
	t.remove("hell");
	std::cout << "expected :" << std::endl << "fmi - > 2 \ngrades - > 2  \nhight - > 1 \nhistory - > 1  \nhi - > 1 \nmemory - > 1 \n" << std::endl;

	std::cout << "got :" << std::endl;
	t.print();
	#endif

	#ifdef TEST_GET_VALUE
	std::cout << "\n\tTEST GET VALUE" << std::endl;
	std::cout << "The value of 'hi' is: expected '3' actually '" << t.getValue("hi") << "'\n";
	if (t.getValue("hi")==3)
	std::cout << "The test is successful\n";
	else
	std::cout << "The test is not successful\n";

	std::cout << "The value of 'graduation' is: expected '0' actually '" << t.getValue("graduation") << "'\n";
	if (t.getValue("graduation") == 0)
	std::cout << "The test is successful\n";
	else
	std::cout << "The test is not successful\n";
	#endif
	#ifdef TEST_FIND
	std::cout << "\n\tTEST FIND" << std::endl;
	if (!t.find("achievement"))
	{
	std::cout << "The test1 is successful\n";
	if (t.find("fmi"))
	std::cout << "The test2 is successful\n";
	else
	std::cout << "The test2 is not successful\n";
	}

	else
	std::cout << "The test1 is not successful\n";

	#endif


	return 0;

}

