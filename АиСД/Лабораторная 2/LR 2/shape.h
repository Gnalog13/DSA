#include <list>
#include <iostream>

using namespace std;

struct OutError: runtime_error // ����� �� ������� ������
{
  OutError(const std::string& s): runtime_error(s.c_str( )) { }
};

struct RotationError: runtime_error // ������� ���������� ��������
{
  RotationError(const std::string& s): runtime_error(s.c_str( )) { }
};

//==1. ��������� ������ � ����� ������� �������� ==
char screen[YMAX][XMAX];
enum color { black = '*', white = '.' };
void screen_init( )
{
  for (auto y = 0; y < YMAX; ++y)
    for (auto &x : screen[y])  x = white;
}
void screen_destroy( )
{
  for (auto y = 0; y < YMAX; ++y)
    for (auto &x : screen[y])  x = black;
}
void on_screen(int a, int b) // �������� ��������� ����� �� �����
{
    if (not(0 <= a && a < XMAX && 0 <= b && b < YMAX))
        throw OutError("����� �� �������:");
}
void put_point(int a, int b)
{ screen[b][a] = black; }
void put_line(int x0, int y0, int x1, int y1)
/* �������� ���������� ��� ������:
��������� ������� ������ �� (x0,y0) �� (x1,y1).
��������� ������: b(x-x0) + a(y-y0) = 0.
�������������� �������� abs(eps), ��� eps = 2*(b(x-x0)) + a(y-y0).  */
{
  int dx = 1;
  int a = x1 - x0;   if (a < 0) dx = -1, a = -a;
  int dy = 1;
  int b = y1 - y0;   if (b < 0) dy = -1, b = -b;
  int two_a = 2*a;
  int two_b = 2*b;
  int xcrit = -b + two_a;
  int eps = 0;
  for (;;) { //������������ ������ ����� �� ������
	  put_point(x0, y0);
	  if (x0 == x1 && y0 == y1) break;
	  if (eps <= xcrit) x0 += dx, eps += two_b;
	  if (eps >= a || a < b) y0 += dy, eps -= two_a;
  }
}
void screen_clear( ) { screen_init( ); } //������� ������
void screen_refresh( ) // ���������� ������
{
  for (int y = YMAX-1; 0 <= y; --y) { // � ������� ������ �� ������
    for (auto x : screen[y])    // �� ������ ������� �� �������
      std::cout << x;
    std::cout << '\n';
  }
}
//== 2. ���������� ����� ==
struct shape { // ����������� ������� ����� "������"
  static list<shape*> shapes;// ������ ����� (���� �� ��� ������!)
  shape( ) { shapes.push_back(this); } //������ �������������� � ������
  virtual point north( ) const = 0;	//����� ��� ��������
  virtual point south( ) const = 0;
  virtual point east( ) const = 0;
  virtual point west( ) const = 0;
  virtual point neast( ) const = 0;
  virtual point seast( ) const = 0;
  virtual point nwest( ) const = 0;
  virtual point swest( ) const = 0;
  virtual void draw( ) = 0;		//���������
  virtual void move(int, int) = 0;	//�����������
  virtual void resize(int) = 0;    	//��������� �������
  virtual ~shape( ) { shapes.remove(this); } //����������
};
list<shape*> shape::shapes;   // ���������� ������ �����
void shape_refresh( ) // ����������� ���� ����� �� ������
{
  screen_clear( );
  for (auto p : shape :: shapes) p->draw( ); //������������ ����������!!!
  screen_refresh( );
}
class rotatable : virtual public shape { //������, ��������� � ��������
public:
	virtual void rotate_left( ) = 0;	//��������� �����
	virtual void rotate_right( ) = 0;	//��������� ������
};
class reflectable : virtual public shape { // ������, ���������
public:					     // � ����������� ���������
	virtual void flip_horisontally( ) = 0;	// �������� �������������
	virtual void flip_vertically( ) = 0;	          // �������� �����������
};
class line : public shape {
/* ������� ������ ["w", "e"].
   north( ) ���������� ����� "���� ������ ������� � ��� ������
   �� �����, ��� ����� ��� �������� �����", � �. �. */
protected:
	point w, e;
public:
  line(point a, point b) : w(a), e(b) { };
  line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
  point north( ) const { return point((w.x+e.x)/2, e.y<w.y? w.y : e.y); }
  point south( ) const { return point((w.x+e.x)/2, e.y<w.y? e.y : w.y); }
  point east( ) const { return point(e.x<w.x? w.x : e.x, (w.y+e.y)/2); }
  point west( ) const { return point(e.x<w.x? e.x : w.x, (w.y+e.y)/2); }
  point neast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? w.y : e.y); }
  point seast( ) const { return point(w.x<e.x? e.x : w.x, e.y<w.y? e.y : w.y); }
  point nwest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? w.y : e.y); }
  point swest( ) const { return point(w.x<e.x? w.x : e.x, e.y<w.y? e.y : w.y); }
  void move(int a, int b) 	{ w.x += a; w.y += b; e.x += a; e.y += b; }
  void draw( )
  {
    try
    {
      on_screen(w.x,w.y);
      on_screen(e.x,e.y);
      put_line(w, e);
    }
    catch (OutError &ex)
    {
      std::cout << ex.what() << " line::draw()\n������ ������ �� �������� ������.";
      std::cin.get();
      this->move(XMAX / 2,YMAX / 2);
      put_line(w, e);
    }
  }
  void resize(int d) // ���������� ����� ����� � (d) ���
  { e.x += (e.x - w.x) * (d - 1); e.y += (e.y - w.y) * (d - 1); }
};
// �������������
class rectangle : public rotatable {
/* nw --------- n ----------ne
   |		                 |
   |		                 |
   w	        c            e
   |		                 |
   |		                 |
   sw --------- s --------- se */
protected:
  point sw, ne;
public:
  rectangle(point a, point b) :  sw(a), ne(b) { }
  point north( ) const { return point((sw.x + ne.x) / 2, ne.y); }
  point south( ) const { return point((sw.x + ne.x) / 2, sw.y); }
  point east( ) const { return point(ne.x, (sw.y + ne.y) / 2); }
  point west( ) const { return point(sw.x, (sw.y + ne.y) / 2); }
  point neast( ) const { return ne; }
  point seast( ) const { return point(ne.x, sw.y); }
  point nwest( ) const { return point(sw.x, ne.y); }
  point swest( ) const { return sw; }
  void rotate_right() // ������� ������ ������������ se
	{ int w = ne.x - sw.x, h = ne.y - sw.y; //(����������� ������� �� ����)
	  sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;	}
  void rotate_left() // ������� ����� ������������ sw
	{ int w = ne.x - sw.x, h = ne.y - sw.y;
	  ne.x = sw.x + h * 2; ne.y = sw.y + w / 2; }
  void move(int a, int b)
	{ sw.x += a; sw.y += b; ne.x += a; ne.y += b; }
    void resize(int d)
  { ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1); }
  void draw( )
  {
    try
    {
      on_screen(sw.x,sw.y);
      on_screen(ne.x,ne.y);
      put_line(nwest( ), ne);   put_line(ne, seast( ));
      put_line(seast( ), sw);   put_line(sw, nwest( ));
    }
    catch (OutError &ex)
    {
      std::cout << ex.what() << " rectangle::draw()\n������ ������ �� �������� ������.";
      std::cin.get();
      this->move(XMAX / 2, YMAX / 2);
      put_line(nwest( ), ne);   put_line(ne, seast( ));
      put_line(seast( ), sw);   put_line(sw, nwest( ));
    }
  }
};
void up(shape& p, const shape& q) // ��������� p ��� q
{	//��� ������� �������, �� ���� ������! ������������ ����������!!
  point n = q.north( );
  point s = p.south( );
  p.move(n.x - s.x, n.y - s.y + 1);
}

// ������ �������: �������������� �������� - ��������������
class h_circle: public rectangle, public reflectable {
  bool reflected;
public:
	h_circle(point a, point b, bool r=true) : rectangle(a, b), reflected(r) { }
	void draw();
	void flip_horisontally( ) { }; // �������� ������������� (������ �������)
	void flip_vertically( ) { reflected = !reflected; };	// �������� �����������
};
void h_circle :: draw() //�������� ���������� ��� �����������
{   //(�������� ��� �������, ����������� ��������� reflected)
    int x0 = (sw.x + ne.x)/2, y0 = reflected ? sw.y : ne.y;
	int radius = (ne.x - sw.x)/2;
	int x = 0, y = radius, delta = 2 - 2 * radius, error = 0;
    while(y >= 0) { // ���� ���������
	    if(reflected) { put_point(x0 + x, y0 + y*0.7); put_point(x0 - x, y0 + y*0.7); }
	    else { put_point(x0 + x, y0 - y*0.7); put_point(x0 - x, y0 - y*0.7); }
       error = 2 * (delta + y) - 1;
       if(delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
       error = 2 * (delta - x) - 1;
       if(delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
       ++x; delta += 2 * (x - y);  --y;
	   }
}
// ������ �������: �������������� ������� ��������������
void down(shape &p,  const shape &q)
{    point n = q.south( );
     point s = p.north( );
     p.move(n.x - s.x, n.y - s.y - 1); }
// C������ ���������������� ������ - ����������
class myshape : public rectangle { // ��� ������ ��������
     int w, h;			             //        ���������������
     line l_eye; // ����� ���� � ��� ������ �������� �����
     line r_eye; // ������ ����
     line mouth; // ���
  public:
     myshape(point, point);
     void draw( );
     void move(int, int);
     void resize(int) { }
};
myshape :: myshape(point a, point b)
	: rectangle(a, b),	//������������� �������� ������
	  w(neast( ).x - swest( ).x + 1), // ������������� ������
	  h(neast( ).y - swest( ).y + 1), // - ������ � ������� ����������!
	  l_eye(point(swest( ).x + 2, swest( ).y + h * 3 / 4), 2),
	  r_eye(point(swest( ).x + w - 4, swest( ).y + h * 3 / 4), 2),
	  mouth(point(swest( ).x + 2, swest( ).y + h / 4), w - 4)
{ }
void myshape :: draw( )
{
	rectangle :: draw( ); //������ ���� (����� � ��� �������� ����!)
	 int a = (swest( ).x + neast( ).x) / 2;
	 int b = (swest( ).y + neast( ).y) / 2;
	 put_point(point(a, b)); // ��� � ���������� ������ �� �������!
}
void myshape :: move(int a, int b)
{
	 rectangle :: move(a, b);
	 l_eye.move(a, b);  r_eye.move(a, b);
	 mouth.move(a, b);
}

class parallelogram_x : public rectangle, public reflectable // �������������� � �������
{
    int r_rotated = 0, l_rotated = 0;
    point corner1() const { return point((3 * sw.x + ne.x) / 4, ne.y); }
    point corner2() const { return point((5 * ne.x - sw.x) / 4, ne.y); }
    point corner3() const { return point((sw.x + 3 * ne.x) / 4, sw.y); }
    point corner4() const { return point((5 * sw.x - ne.x) / 4, sw.y); }
    point rot_corner1() const { return point(sw.x, (3 * sw.y + ne.y) / 4); }
    point rot_corner2() const { return point(sw.x, (5 * ne.y - sw.y) / 4); }
    point rot_corner3() const { return point(ne.x, (sw.y + 3 * ne.y) / 4); }
    point rot_corner4() const { return point(ne.x, (5 * sw.y - ne.y) / 4); }
    public:
        parallelogram_x(point a, point b) : rectangle(a, b) { }
		parallelogram_x (const parallelogram_x &) = delete;   // ��������� ����������� �����
		parallelogram_x (parallelogram_x &&) = delete;   // ����� � ��������� (�++11)
		parallelogram_x operator = (const parallelogram_x &) const = delete;   // ��������� ������������
		parallelogram_x operator = (parallelogram_x &&) const = delete; // ������������ � ���������
        void flip_horisontally()
        { swap(ne.x, sw.x); };
        void flip_vertically()
        { swap(ne.y, sw.y); };
        void l_rotate()
        { rotate_left(); l_rotated = 1; }
        void r_rotate()
        { rotate_right(); r_rotated = 1; }
        void draw( );
};

void check_rotation(int a, int b) // �������� �������� ��������
{
  if (a + b == 2)
    throw RotationError("��������� �������:");
}

void parallelogram_x :: draw( )
{
    try
    {
        check_rotation(r_rotated, l_rotated);
    }
    catch (RotationError &ex)
    {
      l_rotated = 0;
      std::cout << ex.what() << " draw()\n������ �������� ������.";
      std::cin.get();
    }
    if(r_rotated) // ������� ������
    {
        flip_horisontally();
        put_line(rot_corner1(), rot_corner2());
        put_line(rot_corner3(), rot_corner2());
        put_line(rot_corner4(), rot_corner3());
        put_line(rot_corner4(), rot_corner1());
    }
    else if(l_rotated) // ������� �����
    {
        put_line(rot_corner1(), rot_corner2());
        put_line(rot_corner3(), rot_corner2());
        put_line(rot_corner4(), rot_corner3());
        put_line(rot_corner4(), rot_corner1());
    }
    else
    {
        put_line(corner1(), corner2()); // ��������������
        put_line(corner3(), corner2());
        put_line(corner4(), corner3());
        put_line(corner4(), corner1());
    }
    put_line(west(), east()); // �����
    put_line(south(), north());
}

void down2(shape &p,  const shape &q) // �������� p �� q
{    point n = q.south( );
     point s = p.south( );
     p.move(n.x - s.x, n.y - s.y - 1); }

