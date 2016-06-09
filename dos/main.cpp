#include "Media.h"
int main()
{
    int temp = 1;
    map<int,Media> media;
    cout<<"使用说明："<<endl<<"/*请将要导入的记事本(重命名为file)，与exe文件放在一起./*"<<endl<<endl<<endl;
    while(temp) {
        Load(media);
        Print(media);
        cout<<"请输入操作"<<endl;
        cout<<"\t 1)全部视频已看集数+1 （2)添加已看集数 (3)添加视频 (4)删除视频  "<<endl;
        cin>>temp;
        switch(temp) {
            case 1: All_Add(media);    break;
            case 2: Add_number(media); break;
            case 3: Add_media(media);  break;
            case 4: Delete(media);     break;
            default: cout<<"输入错误"<<endl;
        }
        system("clear");
    }
    return 0;
}
