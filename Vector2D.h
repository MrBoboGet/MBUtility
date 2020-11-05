#pragma once
class Vector2D
{
public:
	long double x;
	long double y;
	Vector2D(long double a, long double b);
	float Length();
	void Rotate(float Degrees);
	Vector2D Normalized();
	float DistanceToPoint(Vector2D Point);
	static float DistanceToPoint(Vector2D a, Vector2D b);
	static float AngleFromXAxis(Vector2D a);
	friend Vector2D operator+(Vector2D a, Vector2D b)
	{
		return(Vector2D(a.x + b.x, a.y + b.y));
	}
	friend Vector2D operator-(Vector2D a, Vector2D b)
	{
		return(Vector2D(a.x - b.x, a.y - b.y));
	}
	friend Vector2D operator* (Vector2D a, float b) {
		return(Vector2D(a.x * b, a.y * b));
	}
	friend Vector2D operator* (float a, Vector2D b) {
		return(b * a);
	}
	~Vector2D();
	Vector2D();
};
float PI = 3.1415926535;
Vector2D::Vector2D(long double a, long double b)
{
	x = a;
	y = b;
}
float Vector2D::Length()
{
	return(sqrt(x * x + y * y));
}
Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}
Vector2D::~Vector2D()
{

}
void Vector2D::Rotate(float Degrees)
{
	auto TempX = x;
	auto TempY = y;
	x = TempX * cos(Degrees * PI / 180) - TempY * sin(Degrees * PI / 180);
	y = TempX * sin(Degrees * PI / 180) + TempY * sin(Degrees * PI / 180);
}
Vector2D Vector2D::Normalized()
{
	//returnerar vektorn fast med längd 1
	//vi returnerar delarna delat med C
	float C = sqrt(x * x + y * y);
	return(Vector2D(x / C, y / C));
}
float Vector2D::DistanceToPoint(Vector2D Point)
{
	return(sqrt((x - Point.x) * (x - Point.x) + (y - Point.y) * (y - Point.y)));
}
float Vector2D::DistanceToPoint(Vector2D a, Vector2D b)
{
	return(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}
float Vector2D::AngleFromXAxis(Vector2D a)
{
	//alla arc vinkel funktioner ger ju alltid dubbletter av värden, men dem mest intuitiva för dessa är Cosiuns. Ger dubbletter på värden efter 180. Vi tar alltså arccos av vinkel
	//och om y sedan är negativt lägger vi till 180-vinkeln
	//TODO implementera en egen arccos funktion
	a = a.Normalized();
	float ArcCosIGrader = acos(a.x) * (180 / 3.1415926535);
	std::cout << ArcCosIGrader << std::endl;
	if (a.y < 0)
	{
		ArcCosIGrader = (360 - ArcCosIGrader);
	}
	std::cout << ArcCosIGrader << std::endl;
	return(ArcCosIGrader);
}