#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;

//////////lzw functions
int Hash(const string &key, int tableSize){
	//hash buat string
	int hashVal = 0;

	for(int i = 0; i < key.length(); i++) {
		hashVal = 20*hashVal + int(key[i]);

	}
	hashVal = hashVal % tableSize;
	
	if(hashVal < 0) {
		hashVal = hashVal + tableSize;
	}
	return hashVal;
}

int Hash(const int &key, int tableSize){
	//hash buat inetger
	int hashVal = key % tableSize;

	return 0;
}

template <class KeyObj>
class HashTable{
	
private: 
	struct HashEntry{ 
		// hash entry (struct buat taruh ke tabel)

		KeyObj key;
		int value;

		HashEntry(const KeyObj &key = KeyObj(), int val = -1): key(key), value(val) { }//default values hash entrynya
	};

	vector<HashEntry> arr;//tabelnya
	int currentEltNum;
	int arrSize;
	
	int findPos(const KeyObj &key) const{
		int pos = Hash(key, arrSize);// index array di return dari hashnya

		while(arr[pos].key != key && arr[pos].value != -1){
			pos++;
			if(pos == arrSize){
				pos = 0;
			}
		}

		return pos;
	}

public:
	HashTable(int tableSize): arr(tableSize), arrSize(tableSize), currentEltNum(0) { }
	
	int FindCode(const KeyObj &key) const{
		int pos = findPos(key); //balikin posisi key
		return arr[pos].value;
	}

	void Insert(const KeyObj &key){
		if(currentEltNum != arrSize){
			int pos = findPos(key);//return posisi yang bisa dimasukan

			if(arr[pos].value != -1){ // kalau sudah di masukkan
				return;
			}

			int value = currentEltNum;
			arr[pos] = HashEntry(key, value);//tambah objek baru 
			currentEltNum++;
		}
	}
};


//////////huff compression functions

typedef struct h_node_t{
	struct h_node_t *left, *right;
	int freq;
	char c;
} *huf_node;
 
struct h_node_t pool[256] = {{0}};
huf_node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];
 
huf_node new_node(int freq, char c, huf_node a, huf_node b){
	huf_node n = pool + n_nodes++;
	if (freq) n->c = c, n->freq = freq;
	else{
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}
 
/* priority queue */
void qinsert(huf_node n){
	int j, i = qend++;
	while ((j = i / 2)){
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
huf_node qremove(){
	int i, l;
	huf_node n = q[i = 1];
 
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend){
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/* walk the tree and put 0s and 1s */
void build_code(huf_node n, char *s, int len){
	static char *out = buf;
	if (n->c){
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
 
	s[len] = '0'; build_code(n->left,  s, len + 1);
	s[len] = '1'; build_code(n->right, s, len + 1);
}
 
void init(const char *s){
	int i, freq[128] = {0};
	char c[16];
 
	while (*s){
		freq[(int)*s++]++;
	}
	for (i = 0; i < 128; i++){
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));
	}
 
	while (qend > 2){
		qinsert(new_node(0, 0, qremove(), qremove()));
	}
 
	build_code(q[1], c, 0);
}
 
void encode(const char *s, char *out){
	while (*s){
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}
   
int main(){
	HashTable<string> table(4096);
	
	string key = "0";
	int tempi=0;
	string in;
	cout<<"Enter input string : ";
	cin>>in;
	int aaa;
	for(int i=0; i <= 255; i++){
		//karakter-karakter buat masuk dalam tabel
		key[0] = char(i);	
		table.Insert(key);

	}

	char c;
	string longestPrefix;
	string newText;
	int code;
	bool check = false;
	
	if(in.length()>0){
		key[0] = in[tempi++];
		check = true;
	}

	cout<<"\nlempel ziv compression : \n\t";
	while(check){
		longestPrefix = key;
		//bikin prefix terpanjang biar sama dengan character terakhir

		if(tempi<in.length()){
			key[0] = in[tempi++];
			newText = longestPrefix + key;
			code = table.FindCode(newText);
			//ngecek code prefix yang di ikutin character

			while(code != -1 && tempi<in.length()){
				//lanjut nyari prefix sampe ga ketemu di tabel
				key[0] = in[tempi++];
				longestPrefix = newText;
				newText = longestPrefix + key;
				code = table.FindCode(newText);
			}
			
			if(code == -1){ // buat cout code longest prefix
				code = table.FindCode(longestPrefix);
				cout << code << " ";
				table.Insert(newText);
				key[0] = newText.at(newText.length()-1);
			}
			else{
				cout << code;
				check = false;
			}
		}
		else{ 
			code = table.FindCode(longestPrefix);
			cout << code;
			check = false;
		}
	}
////huff compression
	cout<<"\n\nhuffman compression : \n\t";

		const char *str = in.c_str();
        char buf[1024];
 
	init(str);

	encode(str, buf);
	cout<<buf<<endl;

//////////////////////////////////
///sh compression	

	return 0;
}
