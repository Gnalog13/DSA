#include "screen.h"
#include "shape.h"
#include "parallelogram_x.h"

int main()
{
    setlocale(LC_ALL, "Rus");
	screen_init();
//== 1.���������� ������ ����� ==
	rectangle hat(point(0, 0), point(14, 5));
	line brim(point(0,15),17); // ������
	myshape face(point(15,10), point(27,18));
	h_circle beard(point(40,10), point(50,20));
	parallelogram_x par1(point(35,25), point(47,31));
	parallelogram_x par2(point(58,25), point(70,31));
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //�������� �������� �����
//== 2.���������� � ������ ==
	par1.flip_vertically();
	hat.rotate_right();
	brim.resize(2);
	beard.flip_vertically();
    shape_refresh( );
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //�������� ��������� ���������/���������
//== ������������ ��������� ==
	par2.l_rotate();
    shape_refresh();
	std::cout << "=== Flipped... ===\n";
	std::cin.get(); //�������� ��������� ���������/���������
//== 3.������ ����������� ==
	face.move(12, 0); // ���� - � �������� ���������
	up(brim, face);
	up(hat, brim);
	up(par2, hat);
	down(beard, face);
    down2(par1, face);
	shape_refresh( );
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //�������� ���������
	screen_destroy( );
	return 0;
}

