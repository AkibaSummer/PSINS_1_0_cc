#include "KFApp.h"
#include "PSINS.h"

CKFApp kf;
//CFileRdWt dir("\\");

int main() {
    // 处理文件句柄
    CFileRdWt fin("data1.txt", 29), fins("ins.bin", 0), fkf("kf.bin", 0), frk("rk.bin", 0);
    // 三维变量 wm vm :b系下的角度改变量和速度改变量
    CVect3 wm, vm, gpsvn, gpspos;
    // timu 时间戳 ts 时间间隔
    double timu = 0, ts = 0.01;
    // 初始起点位置
    gpspos = CVect3(34.196255 * glv.deg, 108.875677 * glv.deg, 410.70);
    // 初始化状态
    kf.Init16(CSINS(a2qua(CVect3(-0.821, 3.690, 6.960) * glv.deg), O31, gpspos, timu));

    for (int i = 1; i < 2000 * 100; i++) {
        fin.load();
        if (fin.IsEOF()) break;
        timu = fin.buff[0];
        wm = *(CVect3 *) &fin.buff[1];
        vm = *(CVect3 *) &fin.buff[4];
        wm = wm * glv.dps * ts;
        vm = vm * ts;

        if (fin.buff[19] > 0 && timu > 60) {
            gpsvn = *(CVect3 *) &fin.buff[16];
            gpspos = *(CVect3 *) &fin.buff[19];
            gpspos.i *= glv.deg;
            gpspos.j *= glv.deg;
            kf.SetMeas(&gpsvn, &gpspos, timu);
        }
        kf.TDUpdate(&wm, &vm, 1, ts, 10);
        //		kf.Update(&wm, &vm, 1, ts);
        fkf << kf;
        fins << kf.sins;
        frk << kf.Rt;
        if (i % 1000 == 0) printf("%d\n", i / 100);
    }

    return 0;
}
