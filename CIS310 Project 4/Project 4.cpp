// By Corey Maylone
// CIS 310
// Program 4
// FALL 2010

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

void readIn(vector<int> &op1, vector<int> &op2, char &op, ifstream &indata, ofstream &outdata);
int charToInt(char x); // Helper function for readIn
void add(vector<int> &op1, vector<int> op2);
void multi(vector<int> &op1, vector<int> op2, vector<int> orgOp);
void expon(vector<int> &op1, vector<int> op2, vector<int> orgOp);
void sub(vector<int> &op1, vector<int> op2);
void div(vector<int> &op1, vector<int> op2, ofstream &outdata);
double hexToDec(vector<int> x);	// Helper function used in multi,expon, diov

void v(vector<int> x); // debugger

int main()
{
	vector<int> op1;	// first operand
	vector<int> op2;	// secound operand
	char op;			// operator

	ifstream indata;
	ofstream outdata;
	indata.open("input.txt");
	outdata.open("output.txt");

	do
	{
		readIn(op1, op2, op, indata, outdata);
		if(!op1.empty())
		{
			if(op == '+')
			{
				add(op1,op2);
				for(int i = 0; i <= op1.size()-1; i++)		// outputting answer
					outdata << hex << uppercase <<  op1[i];
				outdata << endl;
			}
			else if (op == '-')
			{
				sub(op1,op2);
				for(int i = 0; i <= op1.size()-1; i++)		// outputting answer
					outdata << hex << uppercase <<  op1[i];
				outdata << endl;
			}
			else if(op == '*')
			{
				multi(op1,op2,op1);
				for(int i = 0; i <= op1.size()-1; i++)		// outputting answer
					outdata << hex << uppercase <<  op1[i];
				outdata << endl;
			}

			else if(op == '$')
			{
				expon(op1,op2,op1);
				for(int i = 0; i <= op1.size()-1; i++)		// outputting answer
					outdata << hex << uppercase <<  op1[i];
				outdata << endl;
			}
			else if(op == '/')
				div(op1, op2, outdata);
			else
				cout << "Incorrect Input";
		}
		
	}while(!indata.eof());

	system("pause");
	
	return 0;
}
void readIn(vector<int> &op1, vector<int> &op2, char &op, ifstream &indata, ofstream &outdata)
{
	// readIn reads in the input from file and parses the data
	// PRE: op1,op2,op are initilized, ifstream is open
	// POST: op1,op2,op are filled. op1 and op2 have their char converted into an int

	char temp;
	
	op1.clear();
	op2.clear();

	indata >> temp;
	if(!indata.eof())
	{
		while(isalnum(temp))		// Op1 gets first half of input
		{
			outdata << temp;
			temp = charToInt(temp);
			op1.push_back(temp);
			indata >> temp;
		}

		op = temp;					// Op gets operator input
		outdata << temp;
	
		indata >> temp;
		while(isalnum(temp))		// Op2 getting secound half of input
		{
			outdata << temp;
			temp = charToInt(temp);
			op2.push_back(temp);
			indata >> temp;
		}

		outdata << temp;
	}
	
}
void add(vector<int> &op1, vector<int> op2)
{
	// add sums op1 and op2
	// PRE: op1, op2 are filled and op1 has >= characters than op2
	// POST: op1 holds the summation of op1 + op2

	bool carry = false;			// carry bit
	const int BASE = 16;
	int j = op2.size()-1;	// counter for op2 vector

	for(int i = op1.size()-1; i >= 0; i--)
	{
		op1[i] += op2[j];

		if(op1[i] >= BASE)	// if carry update
		{
			carry = true;
			int k = i;

			while(carry)			// updating array
			{
				op1[k] -= 16;

				if(k == 0)			// adding 1 to beginning of array
				{
					op1.push_back( op1[op1.size()-1] );
					for(int x = op1.size()-2; x > 0; x-- )
						op1[x] = op1[x-1];

					op1[0] = 1;
					carry = false;
				}
				else
				{
					k--;
					op1[k]++;

					if(op1[k] >= BASE)
						carry = true;
					else
						carry = false;
				}
			}	
		} // end of if carry update

		carry = false;
		if(j > 0)
			j--;
		else
			break;
	}	
}
void multi(vector<int> &op1, vector<int> op2, vector<int> orgOp)
{
	// multi multiplys op1 by op2. this is done by calling the add function multiple times.
	int count = hexToDec(op2) -1;	// number of times add needs to be called

	for(int k = 0; k < count; k++)
		add(op1, orgOp);

}
void expon(vector<int> &op1, vector<int> op2, vector<int> orgOp)
{
	// Expon multiplies op1 by it self op2 times.
	int count = hexToDec(op2) - 1;
	for(int i = 0; i < count; i++)
		multi(op1,orgOp,op1);
}
void sub(vector<int> &op1, vector<int> op2)
{
	// sub subtraces Op1 from op2 and stores into op1
	// PRE: op1 and op2 initilized and filled where op1 > op2
	// POST: op1 = op1 - op2

	const int BASE = 16;
	bool borrow =  false;
	int j = op2.size()-1;

	for(int i = op1.size()-1; i >= 0; i--)
	{
		op1[i] -= op2[j];

		if(op1[i] < 0 && i != 0)			// borrowing from neighbor
		{
			int k = i-1;
			borrow = true;

			while(borrow && k >= 0)			
			{
				if(op1[k] == 0)
				{
					op1[k]--;
					k--;
				}
				else
				{
					op1[k]--;
					borrow = false;
					k++;
					//cout << op1[op1.size()-1] << endl;
					for( ; k <= i; k++)	// adding borrowed back into list
						op1[k] += BASE;
				}
			}
		}
		
		borrow = false;
		if(j > 0)
			j--;
		else
			break;
	}
	
	while(op1[0] == 0)	// loop removes non significant zeros from op1
	{
		for(int i = 0; i < op1.size() - 1; i++)
			op1[i] = op1[i+1];

		op1.pop_back();
	}
}
void div(vector<int> &op1, vector<int> op2, ofstream &outdata)
{
	// div dives op1 by op2, it handles its own output to outdata

	int q = 0; // quotient
	int r = 0; // remainder

	while( hexToDec(op1) > hexToDec(op2) )
	{
		sub(op1, op2);
		q++;
	}
	r = hexToDec(op1);

	outdata << "quotient " << q << ",remainder " <<  r << endl;
}
int charToInt(char x)
{
	// charToInt converts a hex character to its proper (int)value
	// PRE: x has char value
	// POST: charToInt returns the int value of hex character

	switch(x)
	{
		case '0' : return 0;
		case '1' : return 1;
		case '2' : return 2;
		case '3' : return 3;
		case '4' : return 4;
		case '5' : return 5;
		case '6' : return 6;
		case '7' : return 7;
		case '8' : return 8;
		case '9' : return 9;
		case 'A' : return 10;
		case 'B' : return 11;
		case 'C' : return 12;
		case 'D' : return 13;
		case 'E' : return 14;
		case 'F' : return 15;
		default	 : return 99;
	}
}
double hexToDec(vector<int> x)
{
	double count = 0;
	int j = 0;

	for(int i = x.size()-1; i >= 0; i--)
	{
		count += x[i]*pow(16.0,j);
		j++;
	}

	return count;
}

void v(vector<int> x) 
{
	for(int i = 0; i < x.size(); i++)
		cout << hex << uppercase << x[i]; 
}
