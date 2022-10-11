#include "Song.h"
#include <io.h>

Song::Song() :head(nullptr), tail(nullptr)
{
	// 곡 리스트 작성
	writeList();

	// 리스트 불러오기
	loadList();
}

Song::~Song()
{
	// 리스트 제거
	removeList();
}

void Song::writeList()
{
	// 임시 파일 열기
	FILE* file = fopen("./data/songlist", "a");

	// 임시 파일 닫기
	fclose(file);

	// 파일 열기
	file = fopen("./data/songlist", "w");

	struct _finddata_t c_file;
	intptr_t hFile;

	// 검색 시작
	if ((hFile = _findfirst(".\\song\\*.*", &c_file)) == -1L)
	{
		CCLOG("No file(s) in that directory!\n");
	}
	else
	{
		do
		{
			// 읽은 파일이 폴더일 때
			if ((c_file.attrib & _A_SUBDIR) && (c_file.name[0] != '.'))
			{
				// 리스트 작성
				fprintf(file, "%s\n", c_file.name);
			}
		} while (_findnext(hFile, &c_file) == 0);

		// 검색 종료
		_findclose(hFile);
	}

	// 파일 닫기
	fclose(file);
}

void Song::loadList()
{
	// 파일 열기
	FILE* file = fopen("./data/songlist", "r");

	int count = 1;

	while (1)
	{
		List *song = new List;

		// 정보 입력
		song->index = count;
		song->name.clear();
		song->prev = NULL;
		song->next = NULL;

		while (1)
		{
			// 미리 읽어오기
			int _temp = fgetc(file);
			if (_temp == '\n' ||
				_temp == EOF)
			{
				break;
			}

			// 파일 커서 돌아오기
			fseek(file, -1L, SEEK_CUR);

			char temp[50];

			// 문자열 불러오기
			if (fscanf(file, "%s", temp) != -1)
			{
				// 띄어쓰기 추가
				if (!song->name.empty())
				{
					song->name += ' ';
				}

				// 문자열 합치기
				song->name = song->name + temp;
			}
			else
			{
				break;
			}
		}

		if (!song->name.empty())
		{
			// 파일 열기
			FILE* fp = fopen(StringUtils::format("./song/%s/%s.txt", song->name.c_str(), song->name.c_str()).c_str(), "r");

			// 정보 입력
			fscanf(fp, "%f %d %d", &(song->duration), &(song->BPM), &(song->level));

			// 파일 닫기
			fclose(fp);

			// 임시 파일 열기
			fp = fopen(StringUtils::format("./data/%s", song->name.c_str(), song->name.c_str()).c_str(), "a");

			// 임시 파일 닫기
			fclose(fp);

			// 파일 열기
			fp = fopen(StringUtils::format("./data/%s", song->name.c_str(), song->name.c_str()).c_str(), "r");

			// 정보 입력
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

			// 파일 닫기
			fclose(fp);

			if (!head)
			{
				// 머리, 꼬리 설정
				head = song;
				tail = song;
			}
			else
			{
				// 정보 입력
				song->prev = tail;

				// 꼬리 설정
				tail->next = song;
				tail = song;
			}
		}
		else
		{
			// 동적 할당 해제
			delete song;

			// 파일 닫기
			fclose(file);

			// 종료
			return;
		}

		// 카운트 증가
		count++;
	}
}

Song::List* Song::searchList(int _index)
{
	List* temp;

	// 앞 쪽일 때
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
	// 뒷 쪽일 때
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