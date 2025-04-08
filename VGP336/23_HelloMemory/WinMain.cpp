#include "SpringEngine/Inc/SpringEngine.h"

using namespace SpringEngine;
using namespace SpringEngine::Core;

static int sUniqueId = 0;
class Student
{
public:
	Student() 
	{
		mId = ++sUniqueId;
		mName = "NoName" + std::to_string(mId);
	}

	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{

	}

	void Log()
	{
		LOG("Name: %s, Id: %d", mName.c_str(), mId);
	}

	void SetName(const std::string&& name)
	{
		mName = name;
	}

private:
	int mId = 0;
	std::string mName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypeAllocator studentPool = TypeAllocator<Student>("StudentPool", 100);

	std::vector<Student*> students;

	//Test new
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* newStudent = studentPool.New();
		students.push_back(newStudent);
	}

	//Test deletion
	for (uint32_t i = 0; i < 60; ++i)
	{
		Student* student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	//Test Adding More
	for (uint32_t i = 0; i < 55; ++i)
	{
		std::string newStudentName = "New_Student" + std::to_string(i);
		Student* newStudent = studentPool.New(newStudentName);
		newStudent->Log();
		students.push_back(newStudent);
	}

	//Clear all
	for (Student* student : students)
	{
		studentPool.Delete(student);
	}

	students.clear();

	return 0;
}