#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<vector>
#include<sstream>
#include<queue>
using namespace std;

struct Car{
	string Name;
	double completed;
	string curDept;
	int curTime;
	string code;
	bool isCompleted;
	int startTime;
	bool passFrame;
	bool passPaint;
	bool passEngine;
	bool passElectronics;
	bool passIndoor;
	bool passTest;

};

class Node
{
public:
	int time;
	string name;
	Car curr;
	string processed[10];
	int pIndex;
	Node * next;
	Node * child;

	Node()
	{
		time = 0;
		pIndex = 0;
		next = 0;
		child = 0;
	}
	Node(int t,string n)
	{
		time = t;
		pIndex = 0;
		name = n;
		child = 0;
		next = 0;
	}

};


class Factory
{
public:
	Node  * head;

	Factory()
	{
		head = 0;
	}

	Factory(const Factory & obj)
	{
		head = 0;
		Node  * p = obj.head;
		while (p != 0)
		{
			this->inserAtTail(p->time,p->name);
			p = p->next;
		}
	}

	Node * returnHead()
	{
		return head;
	}





	Node * inserAtTail(int  time, string name)
	{
		if (head == 0)
		{
			Node  *p = new Node (time,name);

			head = p;
			p->next = 0;
			return p;
		}
		Node  *p = new Node (time,name);

		Node  * r = head;
		while (r->next != 0)
		{
			r = r->next;
		}

		r->next = p;
		p->next = 0;

		return p;
	}

	Node * insertChild(Node * c,int time,string name)
	{
		Node * tmp = c;
		while (tmp->child != 0)
		{
			tmp = tmp->child;
		}

		tmp->child = new Node(time, name);
		return tmp;
	}

	void display()
	{
		Node * tmp = head;
		int t = 0;
		while (tmp != 0)
		{
			int i = 1;
			cout << "- " << tmp->name << " " << i;
			Node * c = tmp->child;
			
			while (c != 0)
			{
				i++;
				cout << " " << c->name << " " << i;
				c = c->child;
				t++;
			}

			cout << endl;
			int z = t;
			while (z >= 0)
			{
				cout << "\t";
				z--;
			}
				

			tmp = tmp->next;
		}
	}


};

void clear(Factory &f)
{
	Node * t = f.returnHead();
	Car c;
	while (t != 0)
	{
		t->curr = c;
		Node * ch = t->child;
		while (ch != 0)
		{
			ch->curr = c;
			ch = ch->child;
		}
		t = t->next;
	}
}

vector<Car> completed;
void printCarReport(vector<string> tokens, Factory &f , vector<Car> cars )
{
	int time = atoi((tokens.at(1)).c_str());
	//int time = (int)tokens.at(1);
	string code = tokens.at(3);
	int timer = 1;
	int cIndex = 0;
	
	
	for (; timer <= time; timer++)
	{
		bool wait = false;
		bool add = false;
		Car c;
		if (wait)
		{
			cIndex--;
		}
		if (cIndex < cars.size())
			 c = cars.at(cIndex);
		c.curTime = 0;
		if (c.startTime <= timer && c.startTime != 0)
		{
			add = true;
		}
		Node * t = f.returnHead();
		Node * t2 = f.returnHead();
		Node * prev = f.returnHead();
		if (add)
		{
			if (t2->curr.code == "")
			{
				c.curDept = t2->name;
				t2->curr = c;
				add = false;
				wait = false;
				cIndex++;
			}
			else
			{
				Node * ch = t2->child;
				while (ch != 0)
				{
					if (ch->curr.code == "")
					{
						c.curDept = ch->name;
						ch->curr = c;
						add = false;
						wait = false;
						cIndex++;
					}
					ch = ch->child;
				}

			}
		} //for Entering in Frame department
		//t = t->next;
		bool recent = false;
		int count = 1;
		while (prev != 0)
		{
			
			if (prev->curr.code != "") //there is a car
			{
				if (prev != t && !recent)
				{
					prev->curr.curTime++;
					prev->curr.completed = prev->curr.completed + (100 / (6.0*prev->time));
				}
				/*if (count == 2 && prev != t)
				{
					prev->curr.curTime++;
					prev->curr.completed = prev->curr.completed + (100 / (6.0*prev->time));
				}*/
				if (prev->curr.curTime >= prev->time)
				{
					if (prev->curr.curDept == "Test")
					{
						prev->curr.isCompleted = true;
						completed.push_back(prev->curr);
						prev->curr.code = "";
						prev->curr.curTime = 0;
						//break;

					}
					else
					{

						if (t->curr.code == "") //empty
						{
							
							prev->curr.curDept = t->name;
							prev->curr.curTime = 0;
							t->curr = prev->curr;
							wait = false;
							Car c;
							prev->curr = c;
							recent = true;

						}
						else if (t->curr.code != "") //look at childs
						{
							Node * inCh = t->child;
							bool tmp = false;
							while (inCh != 0)
							{
								if (inCh->curr.code == "")
								{
									prev->curr.curDept = inCh->name;
									prev->curr.curTime = 0;
									inCh->curr = prev->curr;
									Car c;
									prev->curr = c;
									recent = true;
									tmp = true;
								}
								inCh = inCh->child;
							}
							if (!tmp)
							{
								wait = true;
							}
						}



					}
				}
				else
					recent = false;
				
			}

			Node * tChild = prev->child;
			bool recentChild = false;
			while (tChild != 0)
			{
				
				if (tChild->curr.code != "") //ther is a car
				{
					if (prev != t && !recentChild)
					{
						tChild->curr.curTime++;
						tChild->curr.completed = tChild->curr.completed + (100 / (6.0*tChild->time));

					}
					/*if (count == 2 && prev != t)
					{
						tChild->curr.curTime++;
						tChild->curr.completed = tChild->curr.completed + (100 / (6.0*tChild->time));
					}*/
					if (tChild->curr.curTime >= tChild->time)
					{
						if (tChild->curr.curDept == "Test")
						{
							tChild->curr.isCompleted = true;

							completed.push_back(tChild->curr);
							
							tChild->curr.code = "";
							tChild->curr.curTime = 0;
							//break;

						}
						else
						{
							
							if (t->curr.code == "") //empty
							{
								
								tChild->curr.curDept = t->name;
								tChild->curr.curTime = 0;
								t->curr = tChild->curr;
								Car c;
								wait = false;
								recentChild = true;
								tChild->curr = c;
							}
							else if (t->curr.code != "")
							{
								Node * inCh = t->child;
								bool tmm = false;
								while (inCh != 0)
								{
									if (inCh->curr.code == "")
									{

										tChild->curr.curDept = inCh->name;
										//inCh->curr.completed = inCh->curr.completed + (100 / 6);
										tChild->curr.curTime = 0;
										inCh->curr = tChild->curr;
										Car c;
										tChild->curr = c;
										recentChild = true;
										tmm = true;

									}
									inCh = inCh->child;
								}
								if (!tmm)
								{
									wait = true;
								}
							}
						}
					}
					
				}
				tChild = tChild->child;
			}
			prev = t;
			if (t != 0)
				t = t->next;

			//count++;
		}


	}
	
	
	Car goal;
	int counter=0;
	
	//cout << "Completed Cars\n";
	for (vector<Car>::iterator it = completed.begin(); it < completed.end(); it++)
	{
		if (it->code == code)
		{
			goal = completed.at(counter);
		}
		counter++;
	}

	if (goal.code == "")
	{

		Node * t = f.returnHead();
		while (t != 0)
		{
			//cout << "cars at dept" << t->name << endl;
			//cout << t->curr.Name << " " << t->curr.code << endl;
			if (t->curr.code == code)
			{
				goal = t->curr;

			}
			else
			{

				Node * ch = t->child;
				while (ch != 0)
				{
					//cout << ch->curr.Name << " " << ch->curr.code << endl;
					if (ch->curr.code == code)
					{
						goal = ch->curr;
					}
					ch = ch->child;
				}
			}

			t = t->next;
		}
	}

	if (goal.code == "")
	{
		counter = 0;
		for (vector<Car>::iterator it = cars.begin(); it < cars.end(); it++)
		{
			if (it->code == code)
			{
				goal = cars.at(counter);
			}
			counter++;
		}
	}
	
	Node * t = f.returnHead();

	
	cout << "---------------------\n";
	cout << "| Report for " << code << "|\n";
	cout << "---------------------\n";

	bool dept = false;
	cout << "Frame: ";
	if (goal.curDept == "Frame")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;

	cout << "Paint: ";
	if (goal.curDept == "Paint")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;

	cout << "Engine: ";
	if (goal.curDept == "Engine")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;
	
	cout << "Electronics: ";
	if (goal.curDept == "Electronics")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;

	cout << "Indoor: ";
	if (goal.curDept == "Indoor")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;

	cout << "Test: ";
	if (goal.curDept == "Test")
	{
		cout << goal.curTime << " | ";
		dept = true;
	}
	else if (!dept)
		cout << t->time << "| ";
	else if (dept)
		cout << "0" << " | ";
	t = t->next;
	


	cout << "Start Time: " << goal.startTime << " | ";
	if (goal.completed > 100.0)
		cout << "Completed: 100%" << " | ";
	else
		cout << "Complete: " << goal.completed << "% | ";
	if (goal.isCompleted)
		cout << "Completed |\n\n\n";
	else
		cout << "In Complete |\n\n\n";








}



void printAllDept(vector<string> tokens, Factory &f, vector<Car> cars)
{
	int time = atoi((tokens.at(1)).c_str());
	string code = tokens.at(3);
	int timer = 1;
	int cIndex = 0;


	for (; timer <= time; timer++)
	{
		bool wait = false;
		bool add = false;
		Car c;
		if (wait)
		{
			cIndex--;
		}
		if (cIndex < cars.size())
			c = cars.at(cIndex);
		c.curTime = 0;
		if (c.startTime <= timer && c.startTime != 0)
		{
			add = true;
		}
		Node * t = f.returnHead();
		Node * t2 = f.returnHead();
		Node * prev = f.returnHead();
		if (add)
		{
			if (t2->curr.code == "")
			{
				c.curDept = t2->name;
				t2->curr = c;
				t2->processed[t2->pIndex] = c.Name + " " + c.code;
				t2->pIndex++;
				add = false;
				wait = false;
				cIndex++;
			}
			else
			{
				Node * ch = t2->child;
				while (ch != 0)
				{
					if (ch->curr.code == "")
					{
						c.curDept = ch->name;
						ch->curr = c;
						ch->processed[ch->pIndex] = c.Name + " " + c.code;
						ch->pIndex++;
						add = false;
						wait = false;
						cIndex++;
					}
					ch = ch->child;
				}

			}
		} //for Entering in Frame department
		//t = t->next;
		bool recent = false;
		int count = 1;
		while (prev != 0)
		{

			if (prev->curr.code != "") //there is a car
			{
				if (prev != t && !recent)
				{
					prev->curr.curTime++;
					prev->curr.completed = prev->curr.completed + (100 / (6.0*prev->time));
				}
				/*if (count == 2 && prev != t)
				{
				prev->curr.curTime++;
				prev->curr.completed = prev->curr.completed + (100 / (6.0*prev->time));
				}*/
				if (prev->curr.curTime >= prev->time)
				{
					if (prev->curr.curDept == "Test")
					{
						prev->curr.isCompleted = true;
						completed.push_back(prev->curr);
						prev->curr.code = "";
						prev->curr.curTime = 0;
						//break;

					}
					else
					{

						if (t->curr.code == "") //empty
						{

							prev->curr.curDept = t->name;
							prev->curr.curTime = 0;
							t->curr = prev->curr;
							t->processed[t->pIndex] = prev->curr.Name + " " + prev->curr.code;
							t->pIndex++;
							wait = false;
							Car c;
							prev->curr = c;
							recent = true;

						}
						else if (t->curr.code != "") //look at childs
						{
							Node * inCh = t->child;
							bool tmp = false;
							while (inCh != 0)
							{
								if (inCh->curr.code == "")
								{
									prev->curr.curDept = inCh->name;
									prev->curr.curTime = 0;
									inCh->curr = prev->curr;
									inCh->processed[inCh->pIndex] = prev->curr.Name + " " + prev->curr.code;
									Car c;
									prev->curr = c;
									recent = true;
									tmp = true;
								}
								inCh = inCh->child;
							}
							if (!tmp)
							{
								wait = true;
							}
						}



					}
				}
				else
					recent = false;

			}

			Node * tChild = prev->child;
			bool recentChild = false;
			while (tChild != 0)
			{

				if (tChild->curr.code != "") //ther is a car
				{
					if (prev != t && !recentChild)
					{
						tChild->curr.curTime++;
						tChild->curr.completed = tChild->curr.completed + (100 / (6.0*tChild->time));

					}
					/*if (count == 2 && prev != t)
					{
					tChild->curr.curTime++;
					tChild->curr.completed = tChild->curr.completed + (100 / (6.0*tChild->time));
					}*/
					if (tChild->curr.curTime >= tChild->time)
					{
						if (tChild->curr.curDept == "Test")
						{
							tChild->curr.isCompleted = true;

							completed.push_back(tChild->curr);

							tChild->curr.code = "";
							tChild->curr.curTime = 0;
							//break;

						}
						else
						{

							if (t->curr.code == "") //empty
							{

								tChild->curr.curDept = t->name;
								tChild->curr.curTime = 0;
								t->curr = tChild->curr;
								t->processed[t->pIndex] = tChild->curr.Name + " " + tChild->curr.code;
								t->pIndex++;
								Car c;
								wait = false;
								recentChild = true;
								tChild->curr = c;
							}
							else if (t->curr.code != "")
							{
								Node * inCh = t->child;
								bool tmm = false;
								while (inCh != 0)
								{
									if (inCh->curr.code == "")
									{

										tChild->curr.curDept = inCh->name;
										//inCh->curr.completed = inCh->curr.completed + (100 / 6);
										tChild->curr.curTime = 0;
										inCh->curr = tChild->curr;
										inCh->processed[inCh->pIndex] = tChild->curr.Name + " " + tChild->curr.code;
										inCh->pIndex++;
										Car c;
										tChild->curr = c;
										recentChild = true;
										tmm = true;

									}
									inCh = inCh->child;
								}
								if (!tmm)
								{
									wait = true;
								}
							}
						}
					}

				}
				tChild = tChild->child;
			}
			prev = t;
			if (t != 0)
				t = t->next;

			//count++;
		}


	}


	Node * t = f.returnHead();
	while (t != 0)
	{
		int counter = 1;
		cout << "\n------------------------------------\n";
		cout << "| Report For Department " << t->name << " " << counter << " | \n";
		cout << "------------------------------------\n";

		if (t->curr.code == "")
		{
			cout << "I am Currently Free\n";
		}
		else
		{
			cout << "I am currently Processing: " << t->curr.Name << " " << t->curr.code << endl;
		}

		cout << "Processed Car List \n";
		for (int i = 0; i < t->pIndex; i++)
		{
			if (t->processed[i] != "")
			{

				cout << i + 1 << ": " << t->processed[i] << endl;
			}
		}


		Node *ch = t->child;
		while (ch != 0)
		{
			counter++;
			cout << "\n------------------------------------\n";
			cout << "| Report For Department " << ch->name << " " << counter << " | \n";
			cout << "------------------------------------\n";
			if (ch->curr.code == "")
			{
				cout << "I am Currently Free\n";
			}
			else
			{
				cout << "I am currently Processing: " << ch->curr.Name << " " << ch->curr.code << endl;
			}

			cout << "Processed Car List \n";
			for (int i = 0; i < ch->pIndex; i++)
			{
				if (ch->processed[i] != "")
				{

					cout << i + 1 << ": " << ch->processed[i] << endl;
				}
			}

			ch = ch->child;
			
		}
		t = t->next;
	}




}




int main( int argc , char * argv[])
{
	/*if (argc != 2)
	{
		cout << "Input file is Missing\n";
	}
	else
	{*/
		string line;
		ifstream myfile;
		Factory f;
		vector<Car> cars;
		vector<Car>::iterator it = cars.begin();
		int acc = 1;
		myfile.open(argv[1]);
		if (!myfile.is_open())
		{
			cout << "Error opening file\n";
		}
		else
		{
			while (getline(myfile, line))
			{
				istringstream iss(line);
				vector<string> tokens;
				
				string s;
				while (getline(iss, s, ' ')) {
					cout << s << endl;
					tokens.push_back(s);
				}
				//vector<string> tokens{ istream_iterator<string>{iss},istream_iterator<string>{} };
			
				string cmd = tokens.at(0);	
				if (cmd == "AddDept")
				{
					string number = tokens.at(1);
					string dept = tokens.at(2);
					string time = tokens.at(3);
					int t = atoi(time.c_str());
					
					Node * tmp = f.inserAtTail(t, dept);
					int n = atoi(number.c_str());
					int i = 2;
					Node * c=0;
					if (n >= 2)
					{
						c = f.insertChild(tmp, t, dept);
					}
					while (i < n)
					{
						c = f.insertChild(c, t, dept);
						i++;
					}

					cout << "Department " << dept << " has been created\n";
				}
				else if (cmd == "PrintFactory")
				{
					f.display();
				}
				else if (cmd == "Produce")
				{
					int stime = atoi((tokens.at(1)).c_str());
					string name = tokens.at(2);
					string code = tokens.at(3);
					Car tmp;
					tmp.code = code;
					tmp.completed = 0;
					tmp.isCompleted = false;
					tmp.curTime = 0;
					tmp.Name = name;
					tmp.startTime = stime;
					tmp.passElectronics = false;
					tmp.passEngine = false;
					tmp.passFrame = false;
					tmp.passIndoor = false;
					tmp.passPaint = false;
					tmp.passTest = false;

					cars.push_back(tmp);


					
					
				}
				else if (cmd == "Report")
				{
					int time = atoi((tokens.at(1)).c_str());
					string strTime = tokens.at(1);
					string act = tokens.at(2);
					if (tokens.size() == 4)
					{
						string code = tokens.at(3);
						cout << "Command: " << tokens.at(0) << " " << time << " " << act << " " << code << endl;
						printCarReport(tokens,f,cars);
					}
					

					if (act == "Cars")
					{

						cout << "\n**********All Cars Report********\n\n";
						cout << "Command: " << " Report " << time << " cars \n\n";
						for (vector<Car>::iterator it = cars.begin(); it < cars.end(); it++)
						{
							vector<string> tmp;
							tmp.push_back("Report");
							
							tmp.push_back(strTime);
							tmp.push_back(it->Name);
							tmp.push_back(it->code);
							clear(f);
							printCarReport(tmp, f, cars);
						}
						cout << "\n\n*******************************\n\n";
						
					}
					else if (act == "Departments")
					{
						clear(f);
						cout << "\n************Report Deaprtments************\n\n";
						cout << "Command: " << " Report " << time << " Departments \n\n";
						vector<string> tmp;
						tmp.push_back("Report");

						tmp.push_back(strTime);
						tmp.push_back("Departments");
						tmp.push_back("NULL");
						printAllDept(tmp,f,cars);

						cout << "\n\n*******************************\n\n";

					}

				}
			}
			myfile.close();
		}
	//}
	return 0;
}