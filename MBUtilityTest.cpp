#include "MBMatrix.h"
#include <time.h>
#include <assert.h>
#include <type_traits>
#include "MBVector.h"
#include <variant>

void TestVector_Trivial()
{
    MBUtility::MBVector<int,4> TestVector; 
    TestVector.push_back(0);
    TestVector.push_back(1);
    TestVector.push_back(2);
    TestVector.push_back(3);
    assert(TestVector[0] == 0);
    assert(TestVector[1] == 1);
    assert(TestVector[2] == 2);
    assert(TestVector[3] == 3);
    MBUtility::MBVector<int,4> TestCopy = TestVector;
    assert(TestVector[0] == 0);
    assert(TestVector[1] == 1);
    assert(TestVector[2] == 2);
    assert(TestVector[3] == 3);
    assert(TestCopy[0] == 0);
    assert(TestCopy[1] == 1);
    assert(TestCopy[2] == 2);
    assert(TestCopy[3] == 3);
    TestCopy.push_back(5);
    TestVector.push_back(5);
    auto TestCopyDynamic = TestCopy;
    assert(TestCopy == TestVector);
    assert(TestCopyDynamic == TestCopy);
    MBUtility::MBVector<int,4> TestCopyAssign;
    TestCopyAssign = TestCopyDynamic;
    assert(TestCopyAssign == TestCopyDynamic);

    std::cout<<TestVector[0]<<std::endl;
}
void TestVector()
{
    MBUtility::MBVector<std::string,4> TestVector; 
    TestVector.push_back("test1");
    TestVector.push_back("test2");
    TestVector.push_back("test3");
    TestVector.push_back("test4");
    assert(TestVector[0] == "test1");
    assert(TestVector[1] == "test2");
    assert(TestVector[2] == "test3");
    assert(TestVector[3] == "test4");
    MBUtility::MBVector<std::string,4> TestCopy = TestVector;
    assert(TestVector[0] == "test1");
    assert(TestVector[1] == "test2");
    assert(TestVector[2] == "test3");
    assert(TestVector[3] == "test4");
    assert(TestCopy[0] == "test1");
    assert(TestCopy[1] == "test2");
    assert(TestCopy[2] == "test3");
    assert(TestCopy[3] == "test4");
    TestCopy.push_back("Test5");
    TestVector.push_back("Test5");
    auto TestCopyDynamic = TestCopy;
    assert(TestCopy == TestVector);
    assert(TestCopyDynamic == TestCopy);
    MBUtility::MBVector<std::string,4> TestCopyAssign;
    TestCopyAssign = TestCopyDynamic;
    assert(TestCopyAssign == TestCopyDynamic);

    std::cout<<TestVector[0]<<std::endl;
    TestVector_Trivial();
}
void MetaTests()
{
    std::cout<<"Variant with trivial types trivial: "<<std::is_trivially_copyable<std::variant<char,float,char*>>::value<<std::endl;
    std::cout<<"Variant with non-trivial types trivial: "<<std::is_trivially_copyable<std::variant<std::vector<char>,float,char*>>::value<<std::endl;
    std::cout<<"std::string size "<<sizeof(std::string)<<std::endl;
    std::cout<<"MBVector size "<<sizeof(MBUtility::MBVector<char,4>)<<std::endl;
    std::cout<<"MBVector size "<<sizeof(MBUtility::MBVector<char,4,char>)<<std::endl;
}

int main(int argc,char* argv[])
{
    TestVector();
    MetaTests();
	MBMath::MBDynamicMatrix<size_t> DynamicFib = MBMath::MBDynamicMatrix<size_t>(2);
	MBMath::MBStaticMatrix<size_t, 2, 2> StaticFib;
	StaticFib(0, 0) = 3;
	StaticFib(0, 1) = -1;
	StaticFib(1, 0) = 1;
	StaticFib(1, 1) = 0;

	DynamicFib(0, 0) = 3;
	DynamicFib(0, 1) = -1;
	DynamicFib(1, 0) = 1;
	DynamicFib(1, 1) = 0;

	MBMath::MBStaticMatrix<size_t, 2, 2> CurrentStatic = StaticFib;
	MBMath::MBDynamicMatrix<size_t> CurrentDynamic = DynamicFib;
	const size_t N = 10000;
	clock_t Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		CurrentStatic = CurrentStatic * StaticFib;
	}
	std::cout << "Static time: " << (clock() - Timer) / double(CLOCKS_PER_SEC) << std::endl;
	Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		CurrentDynamic = CurrentDynamic * DynamicFib;
	}
	std::cout << "Dynamic time: " << (clock() - Timer) / double(CLOCKS_PER_SEC) << std::endl;
	std::cout << CurrentStatic << std::endl;
	std::cout << CurrentDynamic << std::endl;

	MBMath::MBStaticVector<size_t, 2> TestVector;
	TestVector[0] = 1;
	TestVector[1] = 2;
	MBMath::MBStaticVector<size_t, 2> TestVector2 = StaticFib * TestVector;
	std::cout << TestVector2 << std::endl;
	std::cout << StaticFib.Transpose() << std::endl;
	std::cout << StaticFib << std::endl;
	MBMath::MBStaticVector3<float> TestVector3(1, 2, 3);
	std::cout << MBMath::CrossProduct(MBMath::MBStaticVector3<float>(3,2,1), TestVector3) << std::endl;
	std::cout << MBMath::MBStaticVector3<float>(3,2,1)*TestVector3 << std::endl;
	std::cout << MBMath::LinearInterpolation(MBMath::MBStaticVector3<float>(0, 0, 0), MBMath::MBStaticVector3<float>(1, 1, 1), 0.5f) << std::endl;

	MBMath::MBDynamicVector3<float> DynamicTest(1,1,1);
	MBMath::MBStaticVector3<float> StaticTest(1,1,1);
	clock_t DynamicVectorTime = 0;
	clock_t StaticVectorTime = 0;
	Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		DynamicTest.Rotate(45, MBMath::MBDynamicVector3<float>(1, 2, 3));
	}
	DynamicVectorTime = clock() - Timer;
	Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		StaticTest.Rotate(45, MBMath::MBStaticVector3<float>(1, 2, 3));
	}
	StaticVectorTime = clock() - Timer;
	std::cout << "DynamicTime: " << DynamicVectorTime / double(CLOCKS_PER_SEC) <<std::endl;
	std::cout << "StaticTime: " << StaticVectorTime / double(CLOCKS_PER_SEC) <<std::endl;

	//MBMatrix4 test
	/*
	MatrixTest = {{1, 2, 3, 4}, {4, 3, 2, 1}, {3, 1, 2, 1}, {4, 4, 1, 1}}
	*/
	MBMath::MBDynamicMatrix4<float> Dynamic4MatrixTest(4);
	MBMath::MBStaticMatrix4<float> StaticMatrix4Test;
	Dynamic4MatrixTest(0, 0) = 1;
	Dynamic4MatrixTest(0, 1) = 2;
	Dynamic4MatrixTest(0, 2) = 3;
	Dynamic4MatrixTest(0, 3) = 4;
	Dynamic4MatrixTest(1, 0) = 4;
	Dynamic4MatrixTest(1, 1) = 3;
	Dynamic4MatrixTest(1, 2) = 2;
	Dynamic4MatrixTest(1, 3) = 1;
	Dynamic4MatrixTest(2, 0) = 3;
	Dynamic4MatrixTest(2, 1) = 1;
	Dynamic4MatrixTest(2, 2) = 2;
	Dynamic4MatrixTest(2, 3) = 1;
	Dynamic4MatrixTest(3, 0) = 4;
	Dynamic4MatrixTest(3, 1) = 4;
	Dynamic4MatrixTest(3, 2) = 1;
	Dynamic4MatrixTest(3, 3) = 1;

	StaticMatrix4Test(0, 0) = 1;
	StaticMatrix4Test(0, 1) = 2;
	StaticMatrix4Test(0, 2) = 3;
	StaticMatrix4Test(0, 3) = 4;
	StaticMatrix4Test(1, 0) = 4;
	StaticMatrix4Test(1, 1) = 3;
	StaticMatrix4Test(1, 2) = 2;
	StaticMatrix4Test(1, 3) = 1;
	StaticMatrix4Test(2, 0) = 3;
	StaticMatrix4Test(2, 1) = 1;
	StaticMatrix4Test(2, 2) = 2;
	StaticMatrix4Test(2, 3) = 1;
	StaticMatrix4Test(3, 0) = 4;
	StaticMatrix4Test(3, 1) = 4;
	StaticMatrix4Test(3, 2) = 1;
	StaticMatrix4Test(3, 3) = 1;
	std::cout<< Dynamic4MatrixTest.GetMatrixData().Determinant() <<std::endl;
	std::cout<< StaticMatrix4Test.GetDeterminant() <<std::endl;


	std::cout << Dynamic4MatrixTest.GetMatrixData().InvertedMatrix() << std::endl;
	std::cout << StaticMatrix4Test.GetInverse() << std::endl;

	clock_t Dynamic4Time = 0;
	clock_t Static4Time = 0;

	Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		Dynamic4MatrixTest.GetMatrixData().InvertedMatrix();
	}
	Dynamic4Time = clock() - Timer;
	Timer = clock();
	for (size_t i = 0; i < N; i++)
	{
		StaticMatrix4Test.GetInverse();
	}
	Static4Time = clock() - Timer;
	std::cout << "Dynamic inverse time: " << Dynamic4Time / double(CLOCKS_PER_SEC)<<std::endl;
	std::cout << "Static inverse time: " << Static4Time / double(CLOCKS_PER_SEC)<<std::endl;

	MBMath::Quaternion<float> Test;
	Test.Slerp({ 1,2,3,4 }, 4);
}
