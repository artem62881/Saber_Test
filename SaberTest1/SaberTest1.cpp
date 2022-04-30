// SaberTest1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <sstream>

using namespace std;

class FileStream {};

string serialized = "";

class ListNode
{
public:
    ListNode* Prev = NULL;
    ListNode* Next = NULL;
    ListNode* Rand = NULL;

    string Data = {};
};

class ListRand
{
public:
    ListNode* Head = NULL;
    ListNode* Tail = NULL;
    unsigned int Count = 0;

	ListRand()
	{
    
	}

	~ListRand()
	{
        Clear();
	}

    void Serialize(FileStream& s)
    {
        string Output;
  
        unsigned int Num = 0;
        ListNode* Current = Head;
        map<ListNode*, unsigned int> ListIndices_Map;
        
        while (Current)
        {
            ListIndices_Map.insert(pair<ListNode*, unsigned int> (Current, Num));
            Current = Current->Next;
            Num++;
        }

        Current = Head;
        while (Current)
        {
            Output.append(to_string(ListIndices_Map[Current->Rand]) + '|' + Current->Data + '|');
            //Output.append(Current->Data + '|');
            Current = Current->Next;
        }   
        
        serialized = Output;
        //cout << output << endl;
       // s.write(Output);
    }

    void Deserialize(const FileStream& s)
    {
        Clear();
        PrintList();

        //string input = s.read();
        const string Input = serialized;

		vector<unsigned int> RandIndices_Vector;

		string StringPiece;
		stringstream Stream(Input);
        unsigned int Num = 0;
		while (getline(Stream, StringPiece, '|'))
		{
			if (Num % 2 == 0)
			{
				RandIndices_Vector.push_back(stoi(StringPiece));
			}
			else
			{
				ListNode* Current = new ListNode;
				Current->Data = StringPiece;

				if (Count != 0)
				{
					Tail->Next = Current;
					Current->Prev = Tail;
				}
				else
				{
					Head = Current;
				}

				Current->Rand = NULL;
				Tail = Current;
				Count++;
			}
            ++Num;
		}
  
        map<unsigned int, ListNode*> ListIndices_Map;
        Num = 0;
        ListNode* Current = Head;
        while (Current)
        {
            ListIndices_Map.insert(pair<unsigned int, ListNode*>(Num, Current));
            Current = Current->Next;
            Num++;
        }

        Current = Head;
        Num = 0;
        while (Current)
        {
            Current->Rand = ListIndices_Map[RandIndices_Vector[Num]];
			Current = Current->Next;
			Num++;
        }

        PrintList();
   
        serialized.clear();
    }

    void AddNode(const string& in)
    {
        ListNode* Current = new ListNode;
        Current->Data = in;

        if (Count != 0)
        {
            Tail->Next = Current;
            Current->Prev = Tail;          
        }
        else
        {
            Head = Current;
        }

        Current->Rand = NULL;
        Tail = Current;
        Count++;
     }

	void PrintList()
	{
        cout << "---------------------------------" << endl;
        ListNode* Current = Head;
        int num = 0;
        
        if (Current == NULL)
        {
            cout << "List is NULL" << endl;
            return;
        }

        while (Current)
        {
            cout << num << ": " << Current->Data << '|' << Current->Rand->Data << ",\n";
            Current = Current->Next;
            ++num;
        }
	}

    void InitRand()
    {
		ListNode* Current = Head;
        random_device rd;

        while (Current)
        {
            mt19937 rng(rd());
            uniform_int_distribution<unsigned int> uni(0, Count - 1);
            unsigned int rand_index = uni(rng);

            ListNode* Temp = Head;
			for (unsigned int i = 0; i <= rand_index; ++i)
			{
				if (i == rand_index)
				{
					Current->Rand = Temp;
					break;
				}
				else
				{
					Temp = Temp->Next;
				}
			}

            Current = Current->Next;
        }
    }

    void Clear()
    {
		ListNode* Current = Head;
		while (Current)
		{
			ListNode* Temp = Current->Next;
			delete Current;
			Current = Temp;
		}

		Head = NULL;
		Tail = NULL;
		Count = 0;
    }
};



int main()
{
    ListRand list;

    std::cout << "Enter count: ";
    unsigned int n;
    cin >> n;
    cin.ignore();

    string s;
    unsigned int i = 0;
    while (i < n)
    {
		cout << i << ": Enter string: ";
		
		getline(cin, s);
        list.AddNode(s);
        i++;
    }

    list.InitRand();
    list.PrintList();

    FileStream fs;
    list.Serialize(fs);

    list.Deserialize(fs);
}

