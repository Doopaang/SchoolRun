#include "Song.h"
#include <io.h>

Song::Song() :head(nullptr), tail(nullptr)
{
	// �� ����Ʈ �ۼ�
	writeList();

	// ����Ʈ �ҷ�����
	loadList();
}

Song::~Song()
{
	// ����Ʈ ����
	removeList();
}

void Song::writeList()
{
	// �ӽ� ���� ����
	FILE* file = fopen("./data/songlist", "a");

	// �ӽ� ���� �ݱ�
	fclose(file);

	// ���� ����
	file = fopen("./data/songlist", "w");

	struct _finddata_t c_file;
	intptr_t hFile;

	// �˻� ����
	if ((hFile = _findfirst(".\\song\\*.*", &c_file)) == -1L)
	{
		CCLOG("No file(s) in that directory!\n");
	}
	else
	{
		do
		{
			// ���� ������ ������ ��
			if ((c_file.attrib & _A_SUBDIR) && (c_file.name[0] != '.'))
			{
				// ����Ʈ �ۼ�
				fprintf(file, "%s\n", c_file.name);
			}
		} while (_findnext(hFile, &c_file) == 0);

		// �˻� ����
		_findclose(hFile);
	}

	// ���� �ݱ�
	fclose(file);
}

void Song::loadList()
{
	// ���� ����
	FILE* file = fopen("./data/songlist", "r");

	int count = 1;

	while (1)
	{
		List *song = new List;

		// ���� �Է�
		song->index = count;
		song->name.clear();
		song->prev = NULL;
		song->next = NULL;

		while (1)
		{
			// �̸� �о����
			int _temp = fgetc(file);
			if (_temp == '\n' ||
				_temp == EOF)
			{
				break;
			}

			// ���� Ŀ�� ���ƿ���
			fseek(file, -1L, SEEK_CUR);

			char temp[50];

			// ���ڿ� �ҷ�����
			if (fscanf(file, "%s", temp) != -1)
			{
				// ���� �߰�
				if (!song->name.empty())
				{
					song->name += ' ';
				}

				// ���ڿ� ��ġ��
				song->name = song->name + temp;
			}
			else
			{
				break;
			}
		}

		if (!song->name.empty())
		{
			// ���� ����
			FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", song->name.c_str(), song->name.c_str()).c_str(), "r");

			// ���� �Է�
			fscanf(fp, "%f %d %d", &(song->duration), &(song->BPM), &(song->level));

			// ���� �ݱ�
			fclose(fp);

			// �ӽ� ���� ����
			fp = fopen(StringUtils::format("./data/%s", song->name.c_str(), song->name.c_str()).c_str(), "a");

			// �ӽ� ���� �ݱ�
			fclose(fp);

			// ���� ����
			fp = fopen(StringUtils::format("./data/%s", song->name.c_str(), song->name.c_str()).c_str(), "r");

			// ���� �Է�
			if (fscanf(fp, "%d", &(song->combo)) == -1)
			{
				song->combo = NULL;
			}
			if (fscanf(fp, "%f", &(song->score)) == -1)
			{
				song->score = NULL;
			}
			if (fscanf(fp, "%d", &(song->fall)) == -1)
			{
				song->fall = NULL;
			}

			// ���� �ݱ�
			fclose(fp);

			if (!head)
			{
				// �Ӹ�, ���� ����
				head = song;
				tail = song;
			}
			else
			{
				// ���� �Է�
				song->prev = tail;

				// ���� ����
				tail->next = song;
				tail = song;
			}
		}
		else
		{
			// ���� �Ҵ� ����
			delete song;

			// ���� �ݱ�
			fclose(file);

			// ����
			return;
		}

		// ī��Ʈ ����
		count++;
	}
}

Song::List* Song::searchList(int _index)
{
	List* temp;

	// �� ���� ��
	if (_index < tail->index / 2)
	{
		temp = head;

		while (1)
		{
			if (temp->index == _index)
			{
				return temp;
			}
			temp = temp->next;
		}
	}
	// �� ���� ��
	else
	{
		temp = tail;

		while (1)
		{
			if (temp->index == _index)
			{
				return temp;
			}
			temp = temp->prev;
		}
	}
}

void Song::removeList()
{
	List* temp = head;
	List* ptemp = head;

	while (temp)
	{
		ptemp = temp->next;
		delete temp;
		temp = ptemp;
	}
}