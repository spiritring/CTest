//
//  main.m
//  CTest
//
//  Created by TSEnel on 13-5-4.
//  Copyright (c) 2013年 TSEnel. All rights reserved.
//
#include <iostream>
#include <string>
#include <map>

using namespace std;

// 全局基类
class TSObject{};

// 全局回调类型定义
typedef void (TSObject::*TpInstFun)(string sBuffer);

// TS事件系统
class TSEvent : public TSObject{
public:
    void RegistEvent(string sEventKey, TSObject* pInst, TpInstFun pFun) {
        m_MapEvent[sEventKey][pInst] = pFun;
    }
    
    void UnRegistEvent(string sEventKey, TSObject* pInst) {
        m_MapEvent[sEventKey].erase(pInst);
    }
    
    void UnRegistEvent(string sEventKey) {
        m_MapEvent.erase(sEventKey);
    }
    
    void SendMessage(string sEventKey, string sBuffer) {
        if (m_MapEvent.count(sEventKey)) {
            map<TSObject*, TpInstFun>& pInstMap = m_MapEvent[sEventKey];

            map<TSObject*, TpInstFun>::iterator iter = pInstMap.begin();
            for (; iter != pInstMap.end(); iter++) {
                TSObject* pInst = iter->first;
                TpInstFun pFun = iter->second;
                (pInst->*pFun)(sBuffer);
            }
        }
        else {
            cout << "没有消息码 : " << sEventKey << endl;
        }
    }
    
public:
    map<string, map<TSObject*, TpInstFun> > m_MapEvent;
};

// 测试类TSGame
class TSGame : public TSObject{
public:
    TSGame(string name) {
        m_Name = name;
    }
    
    void EventLogin(string sBuffer){
        cout <<  m_Name << " : EventLogin : " << sBuffer << endl;
    }
    
    void EventExit(string sBuffer){
        cout << m_Name << " : EventExit : " << sBuffer << endl;
    }
    
public:
    string m_Name;
};

//测试类TSApp
class TSApp : public TSObject{
public:
    TSApp(string name) {
        m_Name = name;
    }
    
    void EventStart(string sBuffer) {
        cout << m_Name << " : " << sBuffer << endl;
    }
    
public:
    string m_Name;
};

// 声明全局事件系统
TSEvent G_EventSys;

// 入口函数
int main(int argc, const char * argv[])
{
    TSGame pG("TS1");
    TSGame pG2("TS2");
    G_EventSys.RegistEvent("Login", &pG, (TpInstFun)&TSGame::EventLogin);
    G_EventSys.RegistEvent("Login", &pG2,(TpInstFun)&TSGame::EventLogin);
    
    TSApp pA("APP1");
    G_EventSys.RegistEvent("Start", &pA, (TpInstFun)&TSApp::EventStart);
    
    G_EventSys.RegistEvent("Exit", &pG, (TpInstFun)&TSGame::EventExit);
    
    G_EventSys.SendMessage("Login", "WWWWW!!!!!***(FD)SFDS!");
    G_EventSys.SendMessage("Start", "WQL!");
    G_EventSys.SendMessage("Exit", "");
    
    G_EventSys.UnRegistEvent("Exit");
    G_EventSys.UnRegistEvent("Exit");
    G_EventSys.SendMessage("Exit", "");

    return 0;
}

