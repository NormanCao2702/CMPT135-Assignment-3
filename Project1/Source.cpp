#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
using namespace std;

template <class T>
class SmarterArray
{
private:
	T* A;
	int size;

public:
	//Constructors
	SmarterArray();
	SmarterArray(const T* A, const int& size);
	SmarterArray(const SmarterArray<T>& L); //Copy constructor
	//Assignment operator
	SmarterArray<T>& operator = (const SmarterArray<T>& L);
	//Destructor
	~SmarterArray();
	//Getters, Setters, operators and other functions
	int getSize() const;
	T& operator[](const int& index) const;
	int find(const T& e) const;
	void append(const T& e);
	bool remove(const T& e);
	SmarterArray<T> operator - (const SmarterArray<T>& L) const;
	//Friend functions need to be explicitely shown as templated even inside the class declaration
	template<class T1> //Some compilers require a different template name
	friend ostream& operator << (ostream& cout, const SmarterArray<T1>& L);
};

template <class T>
SmarterArray<T>::SmarterArray()
{
	this->size = 0;
}

template <class T>
SmarterArray<T>::SmarterArray(const T* A, const int& size)
{
	if (size > 0)
	{
		this->size = size;
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); i++)
			this->A[i] = A[i];
	}
	else
		this->size = 0;
}

template <class T>
SmarterArray<T>::SmarterArray(const SmarterArray<T>& L) //Copy constructor
{
	this->size = L.getSize();
	if (this->getSize() > 0)
	{
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); i++)
			this->A[i] = L[i];
	}
}

template <class T>
SmarterArray<T>& SmarterArray<T> :: operator = (const SmarterArray<T>& L)
{
	//Check for self asssignment. If so, do  nothing.
	if (this == &L)
		return *this;
	//Delete the left hand side object's memory
	this->~SmarterArray();
	//Now copy the right hand side to the left
	this->size = L.getSize();
	if (this->getSize() > 0)
	{
		this->A = new T[this->getSize()];
		for (int i = 0; i < this->getSize(); i++)
			this->A[i] = L[i];
	}
	return *this;
}

template <class T>
SmarterArray<T>::~SmarterArray()
{
	if (this->getSize() > 0)
	{
		delete[] this->A;
		this->size = 0;
	}
}

template <class T>
int SmarterArray<T>::getSize() const
{
	return this->size;
}

template <class T>
T& SmarterArray<T>::operator[](const int& index) const
{
	if (index < 0 && index >= this->getSize())
	{
		cout << "ERROR! Index out of bounds." << endl;
		abort();
	}
	return this->A[index];
}

template <class T>
int SmarterArray<T>::find(const T& e) const
{
	for (int i = 0; i < this->getSize(); i++)
		if (this->A[i] == e)
			return i;	//return index if found
	return -1;	//return -1 to mean not found
}

template <class T>
void SmarterArray<T>::append(const T& e)
{
	//First create a temporary array whose size is this->size+1
	T* temp = new T[this->getSize() + 1];

	//Copy the elements of this->A to temp
	for (int i = 0; i < this->getSize(); i++)
		temp[i] = this->A[i];

	//Copy the element to be appended to temp
	temp[this->getSize()] = e;

	//Delete the existing array this->A
	if (this->getSize() > 0)
		delete[] this->A;

	//Make the array this->A to point to temp and increment the size
	this->A = temp;
	this->size += 1;
}

template <class T>
bool SmarterArray<T>::remove(const T& e)
{
	int index = this->find(e);
	if (index == -1)
		return false;
	else if (this->getSize() == 1)//only one element in the *this object
	{
		delete[] this->A;
		this->size = 0;
		return true;
	}
	else
	{
		//First create a temporary array whose size is this->size-1
		T* temp = new T[this->getSize() - 1];
		//Copy the elements of this->A to temp except the element at index
		for (int i = 0; i < index; i++)
			temp[i] = this->A[i];
		for (int i = index + 1; i < this->getSize(); i++)
			temp[i - 1] = this->A[i];
		//Delete the existing array this->A
		delete[] this->A;
		//Make the array this->A to point to temp and decrement the size
		this->A = temp;
		this->size -= 1;
		return true;
	}
}

template <class T>
SmarterArray<T> SmarterArray<T>::operator - (const SmarterArray<T>& L) const
{
	//Returns the elements of this that are not found in L
	SmarterArray<T> A;
	for (int i = 0; i < this->getSize(); i++)
	{
		int index = L.find(this->A[i]);
		if (index == -1)
			A.append(this->A[i]);
	}
	return A;
}

template <class T1>
ostream& operator << (ostream& cout, const SmarterArray<T1>& L)
{
	cout << "[";
	for (int i = 0; i < L.getSize() - 1; i++)
		cout << L[i] << ", ";
	if (L.getSize() > 0)
		cout << L[L.getSize() - 1];
	cout << "]";
	return cout;
}

template <class K, class V>
class Map
{
private:
	SmarterArray<K> A1;	//The keys of the map
	SmarterArray<V> A2;	//The values of the map

public:
	//Constructors
	Map(); //Default constructor
	Map(const Map<K, V>&); //Copy constructor. Deep copy.

	//Assignment operator
	Map<K, V>& operator = (const Map<K, V>&); //Assignment operator. Memory clean up and deep copy.

	//Destructor
	~Map(); //Destructor.

	//Getters, Setters, operators and other functions
	int getSize() const; //Return the size of the map
	int findKey(const K&) const; //Return the index of the first element of the Keys array == the argument. Return -1 if not found.
	int findValue(const V&) const; //Return the index of the first element of the Values array == the argument. Return -1 if not found.
	K getKey(const V&) const; //Assert the argument is found in the Values array and then return the first key with the given value 
	V getValue(const K&) const; //Assert the argument is found in the Keys array and then return the first value with the given key
	K getKeyAtIndex(const int&) const; //Assert the index argument and then return the key at the given index
	V getValueAtIndex(const int&) const; //Assert the index argument and then return the value at the given index
	void setKeyAtIndex(const int&, const K&); //Assert the index argument and then set the key at the given index
	void setValueAtIndex(const int&, const V&); //Assert the index argument and then set the value at the given index
	void append(const K&, const V&); //Append the key-value pair to the calling object

	template <class K1, class V1>
	friend ostream& operator << (ostream&, const Map<K1, V1>&); //Output streaming operator
};

template <class K, class V>
Map<K, V>::Map()
{
	//No code is needed here.
	//The member variables will automatically be initialized as default objects
}

template <class K, class V>
Map<K, V>::Map(const Map<K, V>& M)
{
	A1 = M.A1;
	A2 = M.A2;
}
template <class K, class V>
Map<K, V>& Map<K, V>::operator = (const Map<K, V>& M)
{
	A1 = M.A1;
	A2 = M.A2;
	return *this;
}
template <class K, class V>
Map<K, V>::~Map()
{
	A1.~SmarterArray();
	A2.~SmarterArray();
}
template <class K, class V>
int Map<K, V>::getSize() const
{
	return A1.getSize();
}
template <class K, class V>
int Map<K, V>::findKey(const K& key) const
{
	return A1.find(key);
}
template <class K, class V>
int Map<K, V>::findValue(const V& value) const
{
	return A2.find(value);
}
template <class K, class V>
K Map<K, V>::getKey(const V& value) const
{
	int index = A2.find(value);
	assert(index != -1);
	return A1[index];
}
template <class K, class V>
V Map<K, V>::getValue(const K& key) const
{
	int index = A1.find(key);
	assert(index != -1);
	return A2[index];
}
template <class K, class V>
K Map<K, V>::getKeyAtIndex(const int& index) const
{
	assert(index >= 0 && index < A1.getSize());
	return A1[index];
}
template <class K, class V>
V Map<K, V>::getValueAtIndex(const int& index) const
{
	assert(index >= 0 && index < A2.getSize());
	return A2[index];
}
template <class K, class V>
void Map<K, V>::setKeyAtIndex(const int& index, const K& key)
{
	assert(index >= 0 && index < A1.getSize());
	A1[index] = key;
}
template <class K, class V>
void Map<K, V>::setValueAtIndex(const int& index, const V& value)
{
	assert(index >= 0 && index < A2.getSize());
	A2[index] = value;
}
template <class K, class V>
void Map<K, V>::append(const K& key, const V& value)
{
	A1.append(key);
	A2.append(value);
}
template <class K1, class V1>
ostream& operator << (ostream& cout, const Map<K1, V1>& m)
{
	if (m.getSize() == 0)
		cout << "[Empty Map]" << endl;
	else
	{
		cout << endl;
		cout << "Keys \t\t Values" << endl;
		cout << "==== \t\t ======" << endl;
		for (int i = 0; i < m.getSize(); i++)
			cout << m.A1[i] << "\t\t" << m.A2[i] << endl;
	}
	return cout;
}
/*Course*/
class Course
{
private:
	string name;
	int creditHours;
public:
	Course();
	Course(const string&, const int&);
	string getCourseName() const;
	int getCreditHours() const;
	void setCourseName(const string&);
	void setCreditHours(const int&);
	bool operator == (const Course&) const; //Return true if course names are equal and credit hours are also equal.
	friend ostream& operator << (ostream&, const Course&);
};
Course::Course()
{
	name = "None";
	creditHours = 0;
}
Course::Course(const string& n, const int& hours)
{
	this->name = n;
	this->creditHours = hours;
}
string Course::getCourseName() const
{
	return this->name;
}
int Course::getCreditHours() const
{
	return this->creditHours;
}
void Course::setCourseName(const string& n)
{
	this->name = n;
}
void Course::setCreditHours(const int& hours)
{
	this->creditHours = hours;
}
bool Course::operator == (const Course& c) const
{
	if (c.creditHours == this->creditHours && c.name == this->name)
		return true;
	else
		return false;
}
ostream& operator << (ostream& out, const Course& c)
{
	out << "Course Name = " << c.name << ", Credit Hours = " << c.creditHours;
	return out;
}
/*Date*/
struct Date
{
	int y, m, d;
	//bool operator == (const Date&) const;
};
//bool Date::operator==(const Date& d) const
//{
//	if (d.y == this->y && d.m == this->m && d.d == this->d)
//		return true;
//	else
//		return false;
//}
/*Student*/
class Student
{
private:
	string fn, ln; //first name and last name
	Date dob;
public:
	Student(); //Implemented for you
	Student(const string& firstName, const string& lastName, const Date&);
	string getFirstName() const;
	string getLastName() const;
	Date getDob() const;
	void setFirstName(const string&);
	void setLastName(const string&);
	void setDob(const Date&);
	bool operator == (const Student&) const; //Return true if all the first names, last names, and date of births are equal
	friend ostream& operator << (ostream&, const Student&); //Implemented for you
};
Student::Student()
{
	fn = "None";
	ln = "None";
	dob.y = 0;
	dob.m = 0;
	dob.d = 0;
}
Student::Student(const string& firstName, const string& lastName, const Date& d)
{
	this->fn = firstName;
	this->ln = lastName;
	this->dob = d;
}
string Student::getFirstName() const
{
	return this->fn;
}
string Student::getLastName() const
{
	return this->ln;
}
Date Student::getDob() const
{
	return this->dob;
}
void Student::setFirstName(const string& fn)
{
	this->fn = fn;
}
void Student::setLastName(const string& ln)
{
	this->ln = ln;
}
void Student::setDob(const Date& d)
{
	this->dob = d;
}
bool Student::operator == (const Student& s) const
{
	if (s.dob.d == this->dob.d && s.dob.m == this->dob.m && s.dob.y == this->dob.y && s.fn == this->fn && s.ln == this->ln)
		return true;
	else
		return false;
}
ostream& operator << (ostream& out, const Student& s)
{
	out << "Full Name = " << s.fn << " " << s.ln << ": ";
	out << "DOB (d-m-y) = " << s.dob.d << "-" << s.dob.m << "-" << s.dob.y;
	return out;
}
/*school management system*/
typedef Map<int, char> StudentMap;
class SchoolManagementSystem
{
private:
	SmarterArray<Student> studentList; //A SmarterArray to store the students in the school
	SmarterArray<Course> courseList; //A SmarterArray to store the courses in the school
	SmarterArray<StudentMap> studentMapList; //A SmarterArray to store the students' maps
public:
	//Constructors
	SchoolManagementSystem();
	//Getters
	int getNumberOfRegisteredStudents() const;
	int getNumberOfCoursesOffered() const;
	int findStudent(const string& firstName, const string& lastName) const;
	Student getStudent(const int& studentIndex) const;
	StudentMap getStudentMap(const int& studentIndex) const;
	int findCourse(const string& courseName) const;
	Course getCourse(const int& courseIndex) const;
	double getStudentGPA(const int& studentIndex) const;
	int getTopStudentIndex() const;

	//Setters
	bool registerStudent(const Student& s);
	bool enrolStudent(const int& studentIndex, const int& courseIndex);
	bool assignLetterGrade(const int& studentIndex, const int& courseIndex, const char& letterGrade);
	bool offerCourse(const Course& course);
	void removeStudent(const int& studentIndex);
	bool withdrawStudent(const int& studentIndex, const int& courseIndex);
	void removeCourse(const int& courseIndex);

	//Static functions
	static Student generateRandomStudent();
	static char generateRandomLetterGrade();

	//Friend functions
	friend ostream& operator << (ostream&, const SchoolManagementSystem&);
};
SchoolManagementSystem::SchoolManagementSystem()
{}
/*work*/
/*1*/
int SchoolManagementSystem::getNumberOfRegisteredStudents() const
{
	return this->studentList.getSize();
}
/*2*/
int SchoolManagementSystem::getNumberOfCoursesOffered() const
{
	return this->courseList.getSize();
}
/*3*/
int SchoolManagementSystem::findStudent(const string& firstName, const string& lastName) const
{
	for (int i = 0; i < this->studentList.getSize(); i++)
	{
		if (this->studentList[i].getFirstName() == firstName && this->studentList[i].getLastName() == lastName)
			return i;
	}
	return -1;
}
/*4*/
Student SchoolManagementSystem::getStudent(const int& studentIndex) const
{
	assert(studentIndex >= 0);
	return this->studentList[studentIndex];
}
/*5*/
StudentMap SchoolManagementSystem::getStudentMap(const int& studentIndex) const
{
	assert(studentIndex >= 0);
	return this->studentMapList[studentIndex];
}
/*6*/
int SchoolManagementSystem::findCourse(const string& courseName) const
{
	for (int i = 0; i < this->courseList.getSize(); i++)
	{
		if (this->courseList[i].getCourseName() == courseName)
			return i;
	}
	return -1;
}
/*7*/
Course SchoolManagementSystem::getCourse(const int& courseIndex) const
{
	assert(courseIndex >= 0);
	return this->courseList[courseIndex];
}
/*8*/
double convert_letterGrade(const char& ch)
{
	if (ch == 'A')
		return 4.0;
	else if (ch == 'B')
		return 3.0;
	else if (ch == 'C')
		return 2.0;
	else if (ch == 'D')
		return 1.0;
	else if (ch == 'F')
		return 0.0;
}
double SchoolManagementSystem::getStudentGPA(const int& studentIndex) const
{
	assert(studentIndex >= 0);
	double total_credit_hours = 0.0;
	for (int i = 0; i < this->studentMapList[studentIndex].getSize(); i++)
	{
		if (this->studentMapList[studentIndex].getValueAtIndex(i) == 'N')
			continue;
		else
			total_credit_hours += this->courseList[this->studentMapList[studentIndex].getKeyAtIndex(i)].getCreditHours();
	}
	double total_credit_points = 0.0;
	for (int i = 0; i < this->studentMapList[studentIndex].getSize(); i++)
	{
		if (this->studentMapList[studentIndex].getValueAtIndex(i) == 'N')
			continue;
		else
			total_credit_points += (this->courseList[this->studentMapList[studentIndex].getKeyAtIndex(i)].getCreditHours() * convert_letterGrade(this->studentMapList[studentIndex].getValueAtIndex(i)));
	}
	if (total_credit_points == 0.0)
		return 0.0;
	else
		return total_credit_points / total_credit_hours;
}
/*9*/
int SchoolManagementSystem::getTopStudentIndex() const
{
	assert(this->studentList.getSize() >= 1);
	double max = 0.0;
	int position = 0;
	for (int i = 0; i < this->studentList.getSize(); i++)
	{
		if (this->getStudentGPA(i) > max)
		{
			max = this->getStudentGPA(i);
			position = i;
		}
	}
	return position;
}
/*10*/
bool SchoolManagementSystem::registerStudent(const Student& s)
{
	for (int i = 0; i < this->studentList.getSize(); i++)
	{
		if (s == this->studentList[i])
			return false;
	}
	this->studentList.append(s);
	StudentMap temp;
	this->studentMapList.append(temp);
	return true;
}
/*11*/
bool SchoolManagementSystem::enrolStudent(const int& studentIndex, const int& courseIndex)
{
	assert(studentIndex >= 0 && courseIndex >= 0);
	for (int i = 0; i < this->studentMapList[studentIndex].getSize(); i++)
		if (this->studentMapList[studentIndex].getKeyAtIndex(i) == courseIndex)
			return false;
	this->studentMapList[studentIndex].append(courseIndex, 'N');
	return true;
}
/*12*/
bool SchoolManagementSystem::assignLetterGrade(const int& studentIndex, const int& courseIndex, const char& letterGrade)
{
	assert(studentIndex >= 0 && courseIndex >= 0);
	assert(letterGrade == 'A' || letterGrade == 'B' || letterGrade == 'C' || letterGrade == 'D' || letterGrade == 'F');
	for (int i = 0; i < this->studentMapList[studentIndex].getSize(); i++)
	{
		if (this->studentMapList[studentIndex].findKey(courseIndex) != -1)
		{
			this->studentMapList[studentIndex].setValueAtIndex(this->studentMapList[studentIndex].findKey(courseIndex), letterGrade);
			return true;
		}
	}
	return false;
	
}
/*13*/
bool SchoolManagementSystem::offerCourse(const Course& course)
{
	for (int i = 0; i < this->courseList.getSize(); i++)
	{
		if (course == this->courseList[i])
			return false;
	}
	this->courseList.append(course);
	return true;
}
/*14*/
void SchoolManagementSystem::removeStudent(const int& studentIndex)
{
	assert(studentIndex >= 0);
	this->studentList.remove(this->studentList[studentIndex]);
	SmarterArray<StudentMap> temp;
	for (int i = 0; i < studentIndex; i++)
	{
		temp.append(this->studentMapList[i]);
	}
	for (int i = studentIndex + 1; i < this->studentMapList.getSize(); i++)
	{
		temp.append(this->studentMapList[i]);
	}
	this->studentMapList = temp;
}
/*15*/
bool SchoolManagementSystem::withdrawStudent(const int& studentIndex, const int& courseIndex)
{
	assert(studentIndex >= 0 || courseIndex >= 0);
	if (this->studentMapList[studentIndex].findKey(courseIndex)!=-1)
	{
		StudentMap temp;
		for (int i = 0; i < this->studentMapList[studentIndex].findKey(courseIndex); i++)
		{
			temp.append(this->studentMapList[studentIndex].getKeyAtIndex(i), this->studentMapList[studentIndex].getValueAtIndex(i));
		}
		for (int i = this->studentMapList[studentIndex].findKey(courseIndex) + 1; i < this->studentMapList[studentIndex].getSize(); i++)
		{
			temp.append(this->studentMapList[studentIndex].getKeyAtIndex(i), this->studentMapList[studentIndex].getValueAtIndex(i));
		}
		this->studentMapList[studentIndex] = temp;
		return true;
	}
	return false;
}
/*16*/
void SchoolManagementSystem::removeCourse(const int& courseIndex)
{
	assert(courseIndex >= 0);
	for (int i = 0; i < this->studentList.getSize(); i++)
	{
		this->withdrawStudent(i, courseIndex);
	}
	this->courseList.remove(this->courseList[courseIndex]);
	//decrement
	for (int i = 0; i < this->studentMapList.getSize(); i++)
	{
		for (int j = 0; j < this->studentMapList[i].getSize(); j++)
		{
			if (this->studentMapList[i].getKeyAtIndex(j) > courseIndex)
			{
				this->studentMapList[i].setKeyAtIndex(j, this->studentMapList[i].getKeyAtIndex(j) - 1);
			}
		}
	}
}
ostream& operator << (ostream& out, const SchoolManagementSystem& sms)
{
	out << endl << "Students List" << endl;
	if (sms.studentList.getSize() == 0)
		out << "No student has been registered yet." << endl;
	for (int studentIndex = 0; studentIndex < sms.studentList.getSize(); studentIndex++)
		out << "Student at index " << studentIndex << ": " << sms.studentList[studentIndex] << endl;

	out << endl << "Courses List" << endl;
	if (sms.courseList.getSize() == 0)
		out << "No course has been offered yet." << endl;
	for (int courseIndex = 0; courseIndex < sms.courseList.getSize(); courseIndex++)
		out << "Course at index " << courseIndex << ": " << sms.courseList[courseIndex] << endl;

	cout << endl << "Students Map" << endl;
	if (sms.studentMapList.getSize() == 0)
		out << "No student is enrolled in any course yet." << endl;
	for (int studentIndex = 0; studentIndex < sms.studentMapList.getSize(); studentIndex++)
	{
		out << "Student at index " << studentIndex << endl;
		out << sms.studentMapList[studentIndex];
		out << "GPA = " << sms.getStudentGPA(studentIndex) << endl << endl;
	}
	return out;
}
Student SchoolManagementSystem::generateRandomStudent()
{
	string fn, ln;
	Date dob;

	fn = rand() % 26 + 65;
	for (int i = 0; i < 9; i++)
		fn += char(rand() % 26 + 97);

	ln = rand() % 26 + 65;
	for (int i = 0; i < 9; i++)
		ln += char(rand() % 26 + 97);

	dob.y = 1998 + rand() % 5;
	dob.m = 1 + rand() % 12;
	dob.d = 1 + rand() % 30;

	return Student(fn, ln, dob);
}
char SchoolManagementSystem::generateRandomLetterGrade()
{
	int g = rand() % 11;
	if (g == 0)
		return 'A';
	else if (g <= 2)
		return'B';
	else if (g <= 5)
		return'C';
	else if (g <= 7)
		return 'D';
	else
		return 'F';
}
int main()
{
	cout << "Welcome to Phantom College School Management System" << endl;
	cout << "===================================================" << endl;

	srand(1);
	const int STUDENT_SIZE = 10;
	const int COURSE_SIZE = 16;

	//Prepare the courses offered in the school
	Course course[] = {Course("CMPT120", 2), Course("CMPT130", 3), Course("CMPT135", 4),
		Course("MACM101", 2), Course("CALC151", 3), Course("CALC152", 4),
				  Course("MATH242", 4), Course("ECON101", 3), Course("ECON102", 4),
				  Course("ENGL100", 2), Course("HIST101", 3), Course("PHYS100", 2),
				  Course("PHYS101", 3), Course("PHYS102", 4), Course("CHEM101", 3),
				  Course("CHEM102", 4)};

	//Construct and print a school management system object
	SchoolManagementSystem sms;
	cout << sms << endl;

	//Add courses offered into the system
	for (int i = 0; i < COURSE_SIZE; i++)	//there are 16 courses initially
	{
		bool flag = sms.offerCourse(course[i]);
		if (flag)
			cout <<"Course offering successful." << endl;
		else
			cout << "Course offering failed. The same course already exists in the system." << endl;
	}
	cout << endl << "Some courses have been offered and added in to the system." << endl;
	cout << "The updated system information is..." << endl;
	cout << sms << endl;

	//See if CMPT135 course is offered in the school
	int courseIndex = sms.findCourse("CMPT225");
	if (courseIndex == -1)
		cout << "CMPT225 course is not offered in the school." << endl << endl;
	else
	{
		cout << "CMPT225 course is offered in the school. Details below..." << endl;
		cout << sms.getCourse(courseIndex) << endl;
	}

	//See if a randomly chosen course is offered in the school
	int randomCourseIndex = rand() % sms.getNumberOfCoursesOffered();
	Course c = sms.getCourse(randomCourseIndex);
	courseIndex = sms.findCourse(c.getCourseName());
	if (courseIndex == -1)
		cout << c.getCourseName() << " course is not offered in the school." << endl << endl;
	else
	{
		cout << c.getCourseName() << " course is offered in the school. Details below..." << endl;
		cout << sms.getCourse(courseIndex) << endl << endl;
	}

	//Register some new students into the system
	for (int i = 0; i < STUDENT_SIZE; i++)
	{
		bool flag = sms.registerStudent(Student(sms.generateRandomStudent()));
		if (flag)
			cout <<"Student registration successful." << endl;
		else
			cout << "Student registration failed. The same student already exists in the system." << endl;
	}
	cout << endl << "Some students have been registered in to the system." << endl;
	cout << "The updated system information is..." << endl;
	cout << sms << endl;

	//See if Yonas Weldeselassie is a student in the school
	int studentIndex = sms.findStudent("Yonas", "Weldeselassie");
	if (studentIndex == -1)
		cout << "Yonas Weldeselassie is not a student in the school." << endl << endl;
	else
	{
		cout << "Yonas Weldeselassie is a student in the school. Details below..." << endl;
		cout << sms.getStudent(studentIndex) << endl;
		cout << sms.getStudentMap(studentIndex) << endl;
		cout << "GPA = " << sms.getStudentGPA(studentIndex) << endl << endl;
	}

	//See if a randomly chosen student is a student in the school
	int randomStudentIndex = rand() % sms.getNumberOfRegisteredStudents();
	Student s = sms.getStudent(randomStudentIndex);
	studentIndex = sms.findStudent(s.getFirstName(), s.getLastName());
	if (studentIndex == -1)
		cout << s.getFirstName() << " " << s.getLastName() << " is not a student in the school." << endl << endl;
	else
	{
		cout << s.getFirstName() << " " << s.getLastName() << " is a student in the school. Details below..." << endl;
		cout << sms.getStudent(studentIndex) << endl;
		cout << sms.getStudentMap(studentIndex);
		cout << "GPA = " << sms.getStudentGPA(studentIndex) << endl << endl;
	}

	//Enroll students to some courses
	for (int studentIndex = 0; studentIndex < sms.getNumberOfRegisteredStudents(); studentIndex++)
	{
		int numberOfCourses = rand() % (sms.getNumberOfCoursesOffered()/2);
		for (int i = 0; i < numberOfCourses; i++)
		{
			int courseIndex = rand() % sms.getNumberOfCoursesOffered();
			sms.enrolStudent(studentIndex, courseIndex);
		}
	}
	cout << endl << "Some students have been enrolled in to some courses." << endl;
	cout << "The updated system information is..." << endl;
	cout << sms << endl;

	//Assign letter grades to the students
	for (int studentIndex = 0; studentIndex < sms.getNumberOfRegisteredStudents(); studentIndex++)
	{
		if (rand() % 4 == 0)
		{
			cout << "Skipping a student from letter grade assignments." << endl;
			continue;
		}
		int n = sms.getStudentMap(studentIndex).getSize(); //Assign letter grades to each course the student is enrolled in
		for (int j = 0; j < n; j++)
		{
			if (rand() % 5 == 0)
			{
				cout << "Skipping a course from letter grade assignment." << endl;
				continue;
			}
			int courseIndex = sms.getStudentMap(studentIndex).getKeyAtIndex(j);
			sms.assignLetterGrade(studentIndex, courseIndex, sms.generateRandomLetterGrade());
		}
	}
	cout << endl << "Students have been assigned letter grades." << endl;
	cout << "The updated system information is..." << endl;
	cout << sms << endl;

	//The operations below are performed several times in order to test our work rigorously.
	for (int i = 0; i < STUDENT_SIZE/2; i++)
	{
		if (sms.getNumberOfRegisteredStudents() == 0)
			continue;	//There is no any student in the system so we don't need to do anything

		//Remove a randomly chosen student from the system
		randomStudentIndex = rand() % sms.getNumberOfRegisteredStudents();
		sms.removeStudent(randomStudentIndex);
		cout << endl << "The student at index " << randomStudentIndex << " has been removed from the system." << endl;
		cout << "The updated system information is..." << endl;
		cout << sms << endl;

		if (sms.getNumberOfRegisteredStudents() == 0 || sms.getNumberOfCoursesOffered() == 0)
			continue;	//There is no any student or any course in the system so we don't need to do anything

		//Withdraw a student from a course
		randomStudentIndex = rand() % sms.getNumberOfRegisteredStudents();
		s = sms.getStudent(randomStudentIndex);
		studentIndex = sms.findStudent(s.getFirstName(), s.getLastName());
		if (studentIndex == -1)
			cout << s.getFirstName() << " " << s.getLastName() << " is not a student in the school." << endl << endl;
		else
		{
			cout << s.getFirstName() << " " << s.getLastName() << " is a student in the school. Details below..." << endl;
			cout << sms.getStudent(studentIndex) << endl;
			cout << sms.getStudentMap(studentIndex) << endl;

			if (sms.getStudentMap(studentIndex).getSize() == 0)
				continue; //The specified student is not enrolled in any course so nothing to do

			int randomCourseMapIndex = rand() % sms.getStudentMap(studentIndex).getSize();
			int courseIndex = sms.getStudentMap(studentIndex).getKeyAtIndex(randomCourseMapIndex);
			bool flag = sms.withdrawStudent(studentIndex, courseIndex);
			if (!flag)
				cout << "Withdrawing the student at index " << studentIndex << " from the course at index " << courseIndex << " failed." << endl << endl;
			else
			{
				cout << "Student at index " << studentIndex << " withdrawn from the course at index " << courseIndex << endl;
				cout << "The updated information for the student is now..." << endl;
				cout << sms.getStudent(studentIndex) << endl;
				cout << sms.getStudentMap(studentIndex) << endl;
			}
		}
	
		//Remove a randomly chosen course from the system
		randomCourseIndex = rand() % sms.getNumberOfCoursesOffered();
		c = sms.getCourse(randomCourseIndex);
		sms.removeCourse(randomCourseIndex);
		cout << c.getCourseName() << " has been removed from the system." << endl;
		cout << "The system has been updated. Below is the updated system information..." << endl;
		cout << sms << endl;

		//Compute and print the top student
		int topStudentIndex = sms.getTopStudentIndex();
		cout << "The top student is..." << endl;
		cout << sms.getStudent(topStudentIndex) << endl;
		cout << "GPA = " << sms.getStudentGPA(topStudentIndex) << endl << endl;
	}
	system("Pause");
	return 0;
}