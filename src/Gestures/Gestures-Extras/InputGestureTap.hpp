/*

    ofxTableGestures (formerly OF-TangibleFramework)
    Developed for Taller de Sistemes Interactius I
    Universitat Pompeu Fabra

    Copyright (c) 2010 Carles F. Julià <carles.fernandez@upf.edu>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef INPUTGESTURETAP_HPP_INCLUDED
#define INPUTGESTURETAP_HPP_INCLUDED

#include "InputGestureDirectFingers.hpp"



class InputGestureTap : public EventClient, public Singleton<InputGestureTap>
{

    float & maxdistance;
    float & maxtime;
    std::map< DirectFinger *,  std::pair < DirectPoint , float > > previous;

public:

    struct TapArgs: public EventArgs
    {
        float x;
        float y;
    };
    ofEvent<TapArgs> Tap;

    InputGestureTap():
        maxdistance(ofxGlobalConfig::getRef("GESTURES:TAP:MAXDISTANCE",0.002f)),
        maxtime(ofxGlobalConfig::getRef("GESTURES:TAP:MAXTIME",0.2f))
        {
            registerEvent(InputGestureDirectFingers::Instance().newCursor,&InputGestureTap::newCursor);
            registerEvent(InputGestureDirectFingers::Instance().removeCursor,&InputGestureTap::removeCursor);
        }

    virtual void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        DirectFinger * f = a.finger;
        float now = ofGetElapsedTimef();
        previous[f]= make_pair(DirectPoint(f->getX(),f->getY()),now);
    }
    virtual void removeCursor(InputGestureDirectFingers::removeCursorArgs & a)
    {
        DirectFinger * f = a.finger;
        if(previous.find(f) != previous.end())
        {
            float now = ofGetElapsedTimef();
            if (previous[f].second > (now - maxtime) &&
                    previous[f].first.getDistance(f) <= maxdistance)
            {
                TapArgs eventargs;
                eventargs.x = f->getX();
                eventargs.y = f->getY();
                eventargs.target = a.target;
                ofNotifyEvent(Tap,eventargs);
            }
            previous.erase(f);
        }
    }
};


///Deprecated
typedef InputGestureTap InputGestueTap;

//template<class Base>
//class CanTap: public Base
//{
//public:
//    //Interface redefined by ofApp
//    void ETap(InputGestureTap::TapArgs & eventargs)
//    {
//        Tap(eventargs.x,eventargs.y);
//    }
//    virtual void Tap(float x, float y) {}
//
//    CanTap()
//    {
//        ofAddListener(InputGestureTap::Instance().Tap,this,&CanTap::ETap);
//    }
//    virtual ~CanTap()
//    {
//        ofRemoveListener(InputGestureTap::Instance().Tap,this,&CanTap::ETap);
//    }
//};



#endif // INPUTGESTURETAP_HPP_INCLUDED
