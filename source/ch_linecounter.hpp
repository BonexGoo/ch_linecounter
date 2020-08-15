#pragma once
#include <service/boss_zay.hpp>

class ch_linecounterData : public ZayObject
{
public:
    ch_linecounterData();
    ~ch_linecounterData();

public:
    void OnBtn1();
    void Parsing(uint08s content);

public:
    String FileName;
    String Result;
};
