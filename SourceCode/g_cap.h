#ifndef _G_CAP_H
#define _G_CAP_H
namespace zhanghongfei
{
    class G_Cap {
    public:
        G_Cap();
        ~G_Cap();
        float g_loop1(float alpha);
        float g_delta1(float alpha);
        float g_loop2(float alpha);
        float g_delta2(float alpha);

    public:
        float g_cap[3][3][10];
    };
}
#endif
