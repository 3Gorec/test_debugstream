#ifndef DEBUGSTREAM_H
#define DEBUGSTREAM_H

#include <iostream>
#include <list>
#ifdef QT_CORE_LIB
    #include <QObject>
#endif

class DebugStreambuf:
#ifdef QT_CORE_LIB
    public QObject,
#endif
    public std::streambuf{
#ifdef QT_CORE_LIB
    Q_OBJECT
#endif

public:
    typedef void (*output_cb)(const char *s, unsigned int n);
    DebugStreambuf(){}    

    int RegisterStream(std::ostream *stream){
        int ret=0;
        bool already_registred=false;
        for(auto it=targets.begin();it!=targets.end();it++){
            if(*it==stream){
                already_registred=true;
                break;
            }
        }
        if(!already_registred){
            targets.push_back(stream);
        }
        else{
            ret=-1;
        }
        return ret;
    }

    int RegisterCallback(DebugStreambuf::output_cb cb){
        int ret=0;
        bool already_registred=false;
        for(auto it=callbacks.begin();it!=callbacks.end();it++){
            if(*it==cb){
                already_registred=true;
                break;
            }
        }
        if(!already_registred){
            callbacks.push_back(cb);
        }
        else{
            ret=-1;
        }
        return ret;
    }

    int UnRegisterStream(std::ostream *stream){
        int ret=-1;
        auto it=targets.begin();
        while(it!=targets.end()){
            if(*it==stream){
                it=targets.erase(it);
                ret=0;
                break;
            }
            else{
                it++;
            }
        }
        return ret;
    }

    int UnRegisterCallback(output_cb cb){
        int ret=-1;
        auto it=callbacks.begin();
        while(it!=callbacks.end()){
            if(*it==cb){
                it=callbacks.erase(it);
                ret=0;
                break;
            }
            else{
                it++;
            }
        }
        return ret;
    }    

protected:
    virtual std::streamsize  xsputn(const char_type* __s, std::streamsize __n) override{
        for(auto it=targets.begin();it!=targets.end();it++){
            (**it)<<std::string(__s,__n);
            (**it).flush();
        }
        for(auto it=callbacks.begin();it!=callbacks.end();it++){
            (*it)(__s,__n);
        }
#ifdef QT_CORE_LIB
        emit(OutputString(__s,__n));
#endif
        return __n;
    }

    virtual int overflow(int c) override {
        if (c == traits_type::eof()){
            return traits_type::eof();
        }

        char_type ch = static_cast<char_type>(c);
        for(auto it=targets.begin();it!=targets.end();it++){
            (**it)<<ch;
            (**it).flush();
        }
        for(auto it=callbacks.begin();it!=callbacks.end();it++){
            (*it)(&ch,1);
        }
#ifdef QT_CORE_LIB
        emit(OutputString(&ch,1));
#endif
        return ch;
    }

    virtual int underflow() override {
        return traits_type::eof();
    }

    virtual int uflow() override{
        return traits_type::eof();
    }

private:
    std::list<std::ostream *> targets;
    std::list<output_cb> callbacks;

#ifdef QT_CORE_LIB
signals:
    void OutputString(const char *s, unsigned int n);
#endif
};

class DebugStream: public std::ostream{
public:
    DebugStream(DebugStreambuf *sb):std::ostream((std::streambuf *)sb){}    
};


#endif // DEBUGSTREAM_H
