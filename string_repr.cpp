//Joseph Pallansch
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
using namespace std;
//creates the nodes used in the linked list and Huffman coding tree
struct node{
	string a;
	int frequency;
	node *rC;
	node *lC;
	node *next;
};
class string_repr{
	int i, j, count, temp;
	string letter, path, basis, buffer;
	node *head, *walker, *stalker, *least, *preLeast, *n;
	ofstream library;
	ifstream sample, coder;
	stringstream ss;
	public:
	//used to determine the path to each character in the tree recursively
	void travel(node *root){
		if(root->lC != NULL){
			path  = path + "0";
			travel(root->lC);
		}
		if(root->rC != NULL){
			path  = path + "1";
			travel(root->rC);
		}
		if(root->a.length() != 0){
			library << root->a << path << "\n";
		}
		if(path.length()>0){
			path = path.substr(0,path.length()-1);
		}
	}

	//travels the tree recursively to find all leaves
	//prints the character and the frequency
	void printFrequencyTable(node *root){
		if(root->lC != NULL){
			printFrequencyTable(root->lC);
		}
		if(root->rC != NULL){
			printFrequencyTable(root->rC);
		}
		if(root->a.length() != 0){
			cout<<root->a<<": "<<root->frequency<<"\n";
		}
	}
	//creates the tree based on the characters in the smaple.txt file
	//formats the tree and outputs it directly into the library.txt file
	void makeCode(){
		//creates list of nodes contatining characters and their frequencies
		sample.open("sample.txt");
		if((sample.rdstate() & ifstream::failbit) != 0){
			cout<<"Could not open file sample.txt\n";
			exit(EXIT_FAILURE);
		}
		while(getline(sample, buffer)){
			basis  = basis + buffer;
		}
		sample.close();
		count = 0;
		letter = basis.substr(0,1);
		i = 0;
		while(i<basis.length()){
			if(basis.substr(i,1).compare(letter) == 0){
				count++;
				basis.erase(i,1);
				i = 0;
			}
			else{
				i++;
			}
		}
		head = new node();
		head->a = letter;
		head->frequency = count;
		n = head;
		while(basis.length() != 0){
			walker = new node();
			letter = basis.substr(0,1);
			i = 0;
			count = 0;
			while(i<basis.length()){
				if(basis.substr(i,1).compare(letter) == 0){
					count++;
					basis.erase(i,1);
					i = 0;
				}
				else{
					i++;
				}
			}
		walker->a  = letter;
		walker->frequency = count;
		n->next  = walker;
		n = walker;
		}
		//converts list into Huffman coding tree by combining lowest frequencies
		while(head->next != NULL){
		i = 0;
		n = new node();
		while(i < 2){
			walker = head;
			least = head;
			preLeast = head;
			while(walker->next != NULL){
				stalker = walker;
				walker = stalker -> next;
				if(walker->frequency < least->frequency){
					least = walker;
					preLeast = stalker;
				}
			}
			if(n->lC == NULL){
				n->lC = least;
			}
			else{
				n->rC = least;
			}
			if(preLeast == least){
				if(least->next == NULL){
					head = NULL;
				}
				else{
					head = least->next;
				}
			}
			else{
				if(least->next == NULL){
					preLeast->next = NULL;
				}
				else{
					preLeast->next = least->next;
				}
			}
			i++;
		}
		n->frequency = n->lC->frequency + n->rC->frequency;
		if(head==NULL){
			head = n;
			n->next = NULL;
		}
		else{
			n->next = head;
			head = n;
		}
		}
		//turns tree into new format to put into library.txt
		library.open("library.txt");
		if((library.rdstate() & ofstream::failbit) != 0){
			cout<<"Could not open file library.txt\n";
			exit(EXIT_FAILURE);
		}
		travel(head);
		library.close();
		cout<<"File is successfully opened.\n";
	}
	//converts characters into the path to character on Huffman tree
	string encode(string w){
		count  = 0;
		temp = w.length()*16;
		cout<<"The encoded text is: ";
		while(w.length()>0){
			i = 1;
			letter = w.substr(0,1);
			coder.open("library.txt");
			if((coder.rdstate() & ifstream::failbit) != 0){
				cout<<"Could not open file library.txt\n";
				exit(EXIT_FAILURE);
			}
			while(getline(coder, buffer) && i){
				if(buffer.substr(0,1).compare(letter) == 0){
					i = 0;
					cout<<buffer.substr(1,buffer.length()-1);
					count += (buffer.length()-1);
				}
			}
			coder.close();
			//leaves characters not found in library unchanged
			if(i){
				cout<<letter;
				count += 16;
			} 
			w.erase(0,1);
		}
		cout<<"\n";
		ss.str("");		
		ss<<"The compression ratio is: "<<count<<":"<<temp;
		w = ss.str();
		return w;
	}
	//changes strings of 1s and 0s to the characters they represent based on paths in coding tree
	//leaves values that are not valid paths to characters in coding tree unchanged
	void decode(string m){
		cout<<"The decoded text is: ";
		while(m.length()>0){
			i = 1;
			j = 1;
		while(i < m.length()+1 && j){
			letter = m.substr(0,i);
			coder.open("library.txt");
			if((coder.rdstate() & ifstream::failbit) != 0){
				cout<<"Could not open file library.txt\n";
				exit(EXIT_FAILURE);
			}
			while(getline(coder,buffer) && j){
				if(letter.compare(buffer.substr(1,buffer.length()-1)) == 0){
					j = 0;
					m.erase(0,i);
					cout<<buffer.substr(0,1);
				}
			}
			coder.close();
			i++;
		}
		//returns values not found in library.txt unchanged
		if(j){
			cout<<m.substr(0,1);
			m.erase(0,1);
		}
		}
		cout<<"\n";
	}
	node* getRoot(){
		return head;
	}
};
int main(){
	string option, message;
	int valid = 1;
	string_repr *test = new string_repr();
	while(valid){
		cout<<"Please choose from the following:\n\n1: Build the library file from sample.txt\n2: Print the character list and the frequency\n3: Encode a message\n4: Decode a message\n";
		getline(cin, option);
		if(option.compare("1") == 0){
			(*test).makeCode();
		}
		else if(option.compare("2") == 0){
			cout<<"FREQUENCY TABLE\n";
			(*test).printFrequencyTable((*test).getRoot());
		}
		else if(option.compare("3") == 0){
			cout<<"Please enter the text to be encoded:\n";
			getline(cin,message);
			cout<<(*test).encode(message)<<"\n";
		}
		else if(option.compare("4") == 0){
			cout<<"Please enter an encoded message:\n";
			getline(cin,message);
			(*test).decode(message);
		}
		else{
			cout<<"Invalid entry\n";
			cout<<"Exiting...\n";
			valid = 0;
		}
	}
}
