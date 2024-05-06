class parallelogram_x : public rectangle, public reflectable // параллелограмм с крестом
{
    int rotated = 0;
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
		parallelogram_x (const parallelogram_x &) = delete;   // фиктивный конструктор копии
		parallelogram_x (parallelogram_x &&) = delete;   // копия с переносом (С++11)
		parallelogram_x operator = (const parallelogram_x &) const = delete;   // фиктивное присваивание
		parallelogram_x operator = (parallelogram_x &&) const = delete; // присваивание с переносом
        void flip_horisontally()
        { swap(ne.x, sw.x); };
        void flip_vertically()
        { swap(ne.y, sw.y); };
        void l_rotate()
        { rotate_left(); rotated = 1; }
        void r_rotate()
        { rotate_right(); rotated = 2; }
        void draw( );
};

void parallelogram_x :: draw( )
{
    if(rotated) // поворот влево
    {
        if (rotated == 2) flip_horisontally(); // поворот вправо
        put_line(rot_corner1(), rot_corner2());
        put_line(rot_corner3(), rot_corner2());
        put_line(rot_corner4(), rot_corner3());
        put_line(rot_corner4(), rot_corner1());
    }
    else
    {
        put_line(corner1(), corner2()); // параллелограмм
        put_line(corner3(), corner2());
        put_line(corner4(), corner3());
        put_line(corner4(), corner1());
    }
    put_line(west(), east()); // крест
    put_line(south(), north());
}

void down2(shape &p,  const shape &q) // наложить p на q
{    point n = q.south( );
     point s = p.south( );
     p.move(n.x - s.x, n.y - s.y - 1); }
