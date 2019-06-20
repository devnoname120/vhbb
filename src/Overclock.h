/**
 * Overclock a section of the code
 * Note: not thread-safe!
 */
class Overclock
{
public:
    Overclock();
    ~Overclock();

private:
    int armClock;
    int busClock;
    int gpuClock;
    int gpuXbarClock;
};
