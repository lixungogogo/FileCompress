#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <assert.h>
#include <string.h>
#include "Heap.h"
#include "HuffmanTree.h"
using namespace std;
//
class FileCompress
{
public:
	void Compress(const char* filename);
	//建哈弗曼，编码
	void Uncompress(const char* filename);
	void GenerateHuffmanCode(HuffmanTreeNode<CharInfo>* root, string code);
protected:
	void WriteConfig(const char* filename)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fcon = fopen(conf.c_str(), "wb");
		for (int i = 0; i < 256; ++i)
		{

			if (_infos[i]._count)
			{
				fputc(_infos[i]._ch, fcon);
				fputc(',', fcon);
				char count[100];
				_itoa(_infos[i]._count, count, 10);
				fputs(count, fcon);
				fputc(',', fcon);
				fputs(_infos[i].code.c_str(), fcon);
				fputc('\n', fcon);
			}
		}
		fclose(fcon);
	}
	void ReadConfig(const char* filename, CharInfo* HNarry)
	{
		string conf(filename);
		conf = conf + "config";
		FILE* fread = fopen(conf.c_str(), "rb");
		if (fread == NULL)
		{
			cout << "打开待压缩文件失败" << endl;
			return;
		}
		char str[100];
		while (fgets(str, 100, fread))
		{
			char* ptr = str;
			unsigned char index = (unsigned char)*ptr;
			if (index == '\n') //换行符  
			{
				HNarry[index]._ch = index;
				fgets(str, 100, fread);
				char* ptr = str;
				ptr++;
				long long count = 0;//字符的权值，出现的次数  
				while (*ptr != ',' && *ptr)
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}
				HNarry[index]._count = count;
				ptr++;
				string code(ptr);
				HNarry[index].code = code;
			}
			else
			{
				HNarry[index]._ch = index;
				ptr += 2;
				long long count = 0;//字符的权值，出现的次数  
				while (*ptr != ',' && *ptr)
				{
					count *= 10;
					count += (*ptr - '0');
					ptr++;
				}
				HNarry[index]._count = count;
				ptr++;
				string code(ptr);
				HNarry[index].code = code;
			}
		}
	}
	int Get_infos_Com(const char* filename)
	{
		int size = 0;
		FILE* fout = fopen(filename, "r");
		assert(fout);
		unsigned char ch = fgetc(fout);
		while (!feof(fout))
		{
			//在windows下回车是'\r\n'的组合，遇到‘\r\n’时屏幕上打印换行
			if (ch == '\r')
			{
				ch = fgetc(fout); //跳过‘\r’  
				if (ch != '\n')
				{
					fseek(fout, -1, SEEK_CUR);
				}
			}
			_infos[ch]._count++;
			_infos[ch]._ch = ch;
			if (_infos[ch]._count == 1)
				size++;
			ch = fgetc(fout);
		}
		fclose(fout);
		return size;
	}
protected:
	CharInfo _infos[256];
};
void FileCompress::GenerateHuffmanCode(HuffmanTreeNode<CharInfo>* root, string code)
{
	if (root == NULL)
		return;
	if (root->_left == NULL && root->_right == NULL)
	{
		_infos[root->_weight._ch].code = code;
	}
	GenerateHuffmanCode(root->_left, code + '0');
	GenerateHuffmanCode(root->_right, code + '1');
}


void FileCompress::Compress(const char* filename)
{
	HuffmanTree mh;
	int size = Get_infos_Com(filename);
	//建树
	CharInfo invaild;
	mh.CreateTree(_infos, size,invaild);
	//编码
	string code;
	GenerateHuffmanCode(mh.GetRoot(), code);

	//配置文件
	string filenameConfig = filename;
	filenameConfig += ".config";
	
	FILE* fout = fopen(filenameConfig.c_str(), "wb");

	assert(fout);
	for (int i = 0; i < 256; i++)
	{
		if (_infos[i]._count != 0)
		{
			fputc(_infos[i]._ch, fout);
			fputc(',', fout);
			fputc(_infos[i]._count+'0',fout);
			fputc('\n',fout);
		}
	}
	fclose(fout);
	//WriteConfig(filename);
	FILE* fin = fopen(filename, "r");
	unsigned char ch = fgetc(fin);

	string filenamecom = filename;
	filenamecom += ".com";

	fout = fopen(filenamecom.c_str(), "w+");
	assert(fout);
	
	unsigned char value = 0;
	int pos = 7;

	while (!feof(fin))
	{
		if (ch == '\r')
		{
			ch = fgetc(fout);
			if (ch != '\n')
			{
				fseek(fout, -1, SEEK_CUR);
			}
		}
		string& code = _infos[ch].code;
		int d = 0;
		for (int i = 0; i < code.size(); i++)
		{
			value <<= 1;
			if ((code[i] - '0') & (1))
			{
				value |= 1;
			}
			else
			{
				value |= 0;
				d = 1;
			}
			pos--;

			if (pos == -1)
			{
				fputc(value, fout);
				if (d = 0)
				{
					cout << 1;
				}
				value = 0;
				d = 0;
				pos = 7;
			}
		}
		ch = fgetc(fin);
	}
	if (pos != -1)
	{
		for (int i = 0; i <= pos; i++)
		{
			value <<= 1;
		}
		fputc(value, fout);
	}
	fclose(fin);
	fclose(fout);
}

void FileCompress::Uncompress(const char* filename)
{
	HuffmanTree ht;
	int size = 0;
	//从配置文件中获取字符信息
	string filenameconfig = filename;
	filenameconfig += ".config";

	FILE* fout = fopen(filenameconfig.c_str(),"r");
	assert(fout);
	unsigned char ch = fgetc(fout);
	long long sum = 0;
	while (!feof(fout))
	{
		_infos[ch]._ch = ch;
		size++;
		char num = fgetc(fout);
		num = fgetc(fout);
		_infos[ch]._count = num - '0';
		sum += _infos[ch]._count;
		ch = fgetc(fout);
		ch = fgetc(fout);
	}
	fclose(fout);
	//建树
	CharInfo invaild;
	//ReadConfig(filename, _infos);
	ht.CreateTree(_infos, size,invaild);

	//从压缩文件取值遍历
	string filenamecom = filename;
	filenamecom += ".com";
	FILE* fin = fopen(filenamecom.c_str(),"r");
	assert(fin);

	string filenameuncom = filename;
	filenameuncom += ".uncom";

	fout = fopen(filenameuncom.c_str(), "w");
	assert(fout);
	HuffmanTreeNode<CharInfo>* temp = ht.GetRoot();
	ch = fgetc(fin);
	int count = 0;
	unsigned int t = 1;
	while ( !feof(fin))//sum != 0)
	{
		int x = 0;
		for (int i = 7; i >= 0; i--)
		{
			x = ch & (t << 7);
			if ((char)ch == EOF)
			{
				cout << 1;
			}
			ch <<= 1;
			if (x == 0)
				temp = temp->_left;
			else if (x == 0x80)
				temp = temp->_right;
			if (temp->_left == NULL && temp->_right == NULL)
			{
				fputc(temp->_weight._ch, fout);
				//sum--;
				cout << temp->_weight._ch;
				temp = ht.GetRoot();
				x = 0;
			}
		}
		ch = fgetc(fin);
	}
	fclose(fin);
	fclose(fout);
}


int main()
{
	FileCompress fc1, fc2;
	/*fc1.Compress("filename.txt");
	fc2.Uncompress("filename.txt");*/
	FILE *f = fopen("filename.txt", "w+");
	assert(f);
	unsigned char ch = fgetc(f);
	while (!feof(f))
	{
	fputc(ch, f);
	ch = fgetc(f);
	}
	fclose(f);
	return 0;
}