#include "stdafx.h"
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <process.h>

using namespace std;

#define HASHSIZE 17333333
#define MAX 300

int charactercount = 0;
int linecount = 0;
int wordcount = 0;

typedef struct wordnode {
	int times;
	char word[MAX];//单词原型
	char wordhash[MAX];//去掉最末尾数字且字母全为小写
	struct wordnode *next;
}wordnode, *wordlist;

typedef struct phrasenode {
	int times;
	wordlist wordpre;//前一个单词
	wordlist wordaft;//后一个单词
	struct phrasenode *next;
}phrasenode, *phraselist;

wordlist wordhash[HASHSIZE];
phraselist phrasehash[HASHSIZE];

wordnode wordtop[11];
phrasenode phrasetop[11];

int wordELFhash(char *str)
{
	//calculate the hashkey of the word
	unsigned long h = 0, g;
	while (*str)
	{
		h = (h << 4) + *str++;
		if ((g = h & 0xf0000000L) != 0)
		{
			h ^= g >> 24;
			h &= ~g;
		}
	}
	return h;
}

int phraseELFhash(char *prePhrase, char *aftPhrase)
{
	//calculate the hashkey of the phrase
	unsigned long h = 0, g;
	while (*prePhrase)
	{
		h = (h << 4) + *prePhrase++;
		if ((g = h & 0xf0000000L) != 0)
		{
			h ^= g >> 24;
			h &= ~g;
		}
	}
	while (*aftPhrase)
	{
		h = (h << 4) + *aftPhrase++;
		if ((g = h & 0xf0000000L) != 0)
		{
			h ^= g >> 24;
			h &= ~g;
		}
	}
	return h;
}

void my_strlwr(char* &revise, char* str, int length)     //定义一个my_strlwr函数
{
	//修正单词，去掉最末尾单词并将大写字母全部转化为小写
	int i;
	for (i = 0; i < length; i++)
	{
		if (str[i] >= 'A'&&str[i] <= 'Z')
		{
			revise[i] = str[i] + 32;
		}
		else
			revise[i] = str[i];
	}
	revise[i] = '\0';
}

wordnode* wordFrequency(char origin[], int length)
{
	//传入单词原字符与修正过的字符
	char *reg = new char[length];
	int key, index;
	wordlist p, q, s;
	p = NULL;
	s = NULL;
	my_strlwr(reg, origin, length);
	key = wordELFhash(reg);
	index = key % HASHSIZE;
	if (wordhash[index] == NULL)
	{
		//查找失败
		p = new wordnode;
		p->times = 1;
		strcpy(p->word, origin);
		strcpy(p->wordhash, reg);
		wordhash[index] = p;
		wordhash[index]->next = NULL;
		return p;
	}
	else
	{
		q = wordhash[index];
		while (q != NULL)
		{
			if (strcmp(q->wordhash, reg) == 0)
			{
				//查找成功
				q->times++;
				if (strcmp(origin, q->word) < 0)
				{
					//字母序比较
					strcpy(q->word, origin);
				}
				return q;
			}
			s = q;
			q = q->next;
		}
		if (q == NULL)
		{
			//查找失败 尾插法
			p = new wordnode;
			p->times = 1;
			strcpy(p->word, origin);
			strcpy(p->wordhash, reg);
			p->next = NULL;
			s->next = p;
			return p;
		}
	}
}

void phraseFrequency(wordlist word1, wordlist word2)
{
	//统计词组频率
	int key, index;
	if (word1 != NULL && word2 != NULL)
	{
		key = phraseELFhash(word1->wordhash, word2->wordhash);
		index = key % HASHSIZE;
		phraselist p, q, s;
		p = NULL;
		s = NULL;
		if (phrasehash[index] == NULL)
		{
			//查找失败
			p = new phrasenode;
			(p->times) = 1;
			p->wordpre = word1;
			p->wordaft = word2;
			phrasehash[index] = p;
			phrasehash[index]->next = NULL;
		}
		else
		{
			q = phrasehash[index];
			while (q != NULL)
			{
				if (strcmp(q->wordpre->wordhash, word1->wordhash) == 0 && strcmp(q->wordaft->wordhash, word2->wordhash) == 0)
				{
					//查找成功
					q->times++;
					break;
				}
				s = q;
				q = q->next;
			}
			if (q == NULL)
			{
				//查找失败  尾插法
				p = new phrasenode;
				p->times = 1;
				p->wordpre = word1;
				p->wordaft = word2;
				p->next = NULL;
				s->next = p;
			}
		}
	}
}

void countNumber(const char *fileName)
{
	FILE *fp;


	char ch;
	char buffer[MAX];
	//char* regular;
	int flag = 0;
	int i, j, k;
	int wordtotal = 0;
	wordlist current;
	wordlist last = NULL;
	printf("%s\n", fileName);
	if ((fp = fopen(fileName, "r")) == NULL)
	{
		printf("The file cannot be opened.\n");
		exit(0);
	}
	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '\n')
		{
			//换行符
			linecount++;
		}

		if (ch >= 32 && ch <= 126)
		{
			//统计ASCII码值在32-126的字符数
			charactercount++;
		}
		
		if ((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z') || (ch>='0'&&ch<='9'))
		{
			//字母与数字
			//charactercount++;
			for (i = 0; ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z') || (ch >= '0'&&ch <= '9')) && ch != EOF; i++)
			{
				charactercount++;
				buffer[i] = ch;
				ch = fgetc(fp);
			}
			charactercount--;
			buffer[i] = '\0';
			if (i >= 4 && ((buffer[0] >= 'a'&&buffer[0] <= 'z') || (buffer[0] >= 'A'&&buffer[0] <= 'Z')) && ((buffer[1] >= 'a'&&buffer[1] <= 'z') || (buffer[1] >= 'A'&&buffer[1] <= 'Z')) && ((buffer[2] >= 'a'&&buffer[2] <= 'z') || (buffer[2] >= 'A'&&buffer[2] <= 'Z')) && ((buffer[3] >= 'a'&&buffer[3] <= 'z') || (buffer[3] >= 'A'&&buffer[3] <= 'Z')))
			{
				wordtotal++;//此时i即为单词原始长度
				for (k = i - 1; ; k--)
				{
					if ((buffer[k] >= 'a'&&buffer[k] <= 'z') || (buffer[k] >= 'A'&&buffer[k] <= 'Z'))
						break;//k represents the last location of a character
				}
				//my_strlwr(regular, buffer, k + 1);
				current = wordFrequency(buffer, k + 1);
				if (wordtotal > 0)
				{
					//不是第一个单词
					phraseFrequency(last, current);
				}
				last = current;
			}
			if (ch >= 32 && ch <= 126)
				charactercount++;
			if (ch == '\n')
			{
				linecount++;
			}
			if (ch == EOF)
			{
				break;
			}
		}
	}
	fclose(fp);
	linecount++;
	wordcount += wordtotal;
	return;
}

void TraverseFolder(string folderPath)
{
	if (folderPath.empty())
	{
		printf("The path is empty.\n");
		exit(0);
	}
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";

	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		cerr << "can not match the folder path" << endl;
		return;
	}
	do {
		//判断是否有子目录  
		if (FileInfo.attrib & _A_SUBDIR)
		{
			//这个语句很重要  
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0))
			{
				string newPath = folderPath + "\\" + FileInfo.name;
				TraverseFolder(newPath);
			}
		}
		else
		{
			//cout << folderPath.c_str() << "\\" << FileInfo.name << endl;
			string fullName;

			fullName = folderPath + "\\" + FileInfo.name;
			countNumber(fullName.c_str());
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
}

void my_sort()
{
	//遍历两个哈希表，找出频率前十的单词和词组
	int i, j;
	wordnode *p;//单词结点指针
	phrasenode *q;//词组结点指针
	for (i = 0; i < 9; i++)
	{
		wordtop[i].times = 0;
		phrasetop[i].times = 0;
	}
	for (i = 0; i < HASHSIZE; i++)
	{
		if ((p = wordhash[i]) != NULL)
		{
			while (p != NULL)
			{
				for (j = 9; j >= 0 && p->times > wordtop[j].times; j--)
				{
					strcpy(wordtop[j + 1].word, wordtop[j].word);
					wordtop[j + 1].times = wordtop[j].times;
				}
				if (j < 9)
				{
					strcpy(wordtop[j + 1].word, p->word);
					wordtop[j + 1].times = p->times;
				}
				p = p->next;
			}
		}
	}
	for (i = 0; i < HASHSIZE; i++)
	{
		if ((q = phrasehash[i]) != NULL)
		{
			while (q != NULL)
			{
				for (j = 9; j >= 0 && q->times > phrasetop[j].times; j--)
				{
					phrasetop[j + 1].wordpre = phrasetop[j].wordpre;
					phrasetop[j + 1].wordaft = phrasetop[j].wordaft;
					phrasetop[j + 1].times = phrasetop[j].times;
				}
				if (j < 9)
				{
					phrasetop[j + 1].wordpre = q->wordpre;
					phrasetop[j + 1].wordaft = q->wordaft;
					phrasetop[j + 1].times = q->times;
				}
				q = q->next;
			}
		}
	}
}

void OutputResult()
{
	//输出文件result.txt
	FILE *fout;
	int i;
	if ((fout = fopen("result.txt", "w")) == NULL)
	{
		printf("cannot open the file.\n");
		exit(0);
	}
	fprintf(fout,"%s:%d\n", "char_number", charactercount);
	fprintf(fout, "%s:%d\n", "line_number", linecount);
	fprintf(fout, "%s:%d\n", "word_number", wordcount);
	fprintf(fout, "\n");

	fprintf(fout, "%s\n", "the top ten frequency of  word:");
	for (i = 0; i < 10; i++)
	{
		fprintf(fout, "%s: %d\n", wordtop[i].word, wordtop[i].times);
	}
	fprintf(fout, "\n");
	
	fprintf(fout, "%s\n", "the top ten frequency of phrase:");
	for (i = 0; i < 10; i++)
	{
		fprintf(fout,"%s %s: %d\n", phrasetop[i].wordpre->word, phrasetop[i].wordaft->word, phrasetop[i].times);
	}
	fprintf(fout, "\n");
}

int main(int argc, const char* argv[])
{
	int i;
	_finddata_t fileInfo;
	/*
	if (argc <= 1)
	{
	return -1;
	}
	long handle = _findfirst(argv[1], &fileInfo);

	if (handle != -1L && (fileInfo.attrib & _A_SUBDIR) == 0)
	{
		//a file
		countNumber(argv[1]);
	}
	else
	{
		//a folder path
	*/
		TraverseFolder("C:\\Users\\97006\\Documents\\test\\newsample");
	//}
	my_sort();
	OutputResult();
	
	return 0;
}

