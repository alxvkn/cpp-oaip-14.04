template <typename T>
class M {
public:
    static T abs(T n) {
        return n < 0 ? -n : n;
    }

    static T gcd(T a, T b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    static T lcm(T a, T b) {
        return abs(a * b) / gcd(a, b);
    }
};
