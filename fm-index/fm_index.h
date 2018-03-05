#ifndef FM_INDEX
#define FM_INDEX

#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

class fm_index{
public:
    static string BWT(string T,int SA[])
    {
        //插入$后并轮转
        int n=T.length();
        T.insert(n,"$");
        string str[n+1];
        str[0]=T;
        for (int i=1;i<=n;i++)
        {
            str[i]=str[i-1].substr(1).insert(n,1,str[i-1].at(0));
        }
        //字典序排序
        sort(str,str+n+1);
        //建立索引
        for (int i=0;i<=n;i++)
        {
            SA[i]=n-str[i].find('$');
        }
        //提取最后一列元素
        string S=T;
        for (int i=0;i<=n;i++)
            S[i]=str[i][n];
        return S;
    }
    static int C(string S,char c)
    {
        int count=0;
        for (unsigned int i=0;i<S.length();i++)
            if (S[i]<c&&S[i]!='$')
                count++;
        return count;
    }
    static int Occ(string S,char c,int r)
    {
        int count=0;
        for (int i=0;i<r;i++)
        {
            if (S[i]==c)
                count++;
        }
        return count;
    }
    static int LF(string S,int r)
    {
        char c=S[r];
        return C(S,c)+Occ(S,c,r);
    }
    static string unpermute(string S)
    {
        //将BWT串还原成原串
        int r=0;
        string T="";
        while (S[r]!='$')
        {
            string x="$";
            x[0]=S[r];
            T.insert(0,x);
            r=LF(S,r);
        }
        return T;
    }
    static int LFC(string S,char c,int r)
    {
        return C(S,c)+Occ(S,c,r)+1;
    }
    static int exactmatch(string S,string P,int &sp,int &ep)
    {
        int i;
        int p=P.length()-1;
        char c=P[p];
        sp=C(S,c)+1;
        ep=C(S,c+1)+1;
    //cout<<"("<<sp<<","<<ep<<")"<<endl;
        for (i=p-1;sp<ep&&i>=0;i--)
        {
            c=P[i];
            sp=LFC(S,c,sp);
            ep=LFC(S,c,ep);
    //cout<<"("<<sp<<","<<ep<<")"<<endl;
        }
        //cout<<"正在匹配子串"<<P<<"......"<<endl;
        if (sp<ep)
        {
            //cout<<"精确匹配区间为：("<<sp<<","<<ep<<")"<<endl;
            return -1;
        }
        else
        {
            //cout<<"not found"<<endl;
            return i+2;
        }
    }

};
#endif // FM_INDEX

