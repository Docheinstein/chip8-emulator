#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

class PowerSupply {
public:
    virtual ~PowerSupply() = default;
    virtual bool on() = 0;
};

#endif // POWERSUPPLY_H