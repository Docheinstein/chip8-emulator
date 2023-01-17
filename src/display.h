#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
public:
    virtual ~Display() = default;
    [[nodiscard]] virtual bool get(unsigned int x, unsigned int y) const = 0;
    virtual void set(unsigned int x, unsigned int y, bool on) = 0;
    virtual void clear() = 0;
    virtual void refresh() = 0;
};

#endif // DISPLAY_H