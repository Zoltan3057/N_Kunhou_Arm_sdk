

第一步准备工作: 客户需要安装虚拟机+Ubuntu14.04 ;
1、首先在Windows下搭建Linux虚拟机，此处选择的是安装VMware。
   在官网下载并打开VM安装包，设置安装路径，按照提示安装即可。
  
2、其次在VMware虚拟机中安装Ubantu系统。
   从官网下载镜像文件ubuntu-14.04.4-desktop-amd64.iso，打开VMware Workstation，选择新建虚拟机，设置安装路径，按照提示安装即可。
   
3、然后是在ubantu中建立Xilinx交叉编译环境(ARM)。

   步骤如下：
   
          步骤1:
          
          打开虚拟机中的 linux 系统（64 位），打开终端输入如下命令
                sudo apt-get install g++
                sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0
                
          步骤2：
          
              (1) 将 文 件 xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin 拷 贝 到 新 建 的 文 件 夹Software 中；
              
              (2)  在终端输入如下命令，注意，输入所有命令均处在用户模式，不要进入 root 用户
                  1) cd Software
                  2) sudo dpkg-reconfigure dash  //弹出提示 选NO
                  3) ls                          // 查看文件属性（文件需要可执行权限） （可忽略该步）
                  4) 修改权限 输入
                     chmod 755 xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin
                  5) ls                         // 查看 xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin 是否为可执行文件（可执行文件显示为绿色） （可忽略该步）
                  6)运行 ./xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin    //执行后按照相关提示引导进行配置，用默认设置的即可
                  7) cd  // 进入用户根目录
                  sudo su
                  8)  sudo gedit .bashrc 会弹出文件编辑
                  在.bashrc最末行添加如下语句：
                  export ARCH=arm
                  export CROSS_COMPILE=arm-xilinx-linux-gnueabi
                  export PATH=/home/taibaidl/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/bin/:$PATH       //usr 是当前用户的名称
                  9) source .bashrc 然后关闭之前开的终端;
                  10) 重新打开终端, echo $PATH //查看路径是否添加上（可忽略该步骤）
                  11) arm-xilinx-linux-gnueabi-gcc -v       //查看版本信息（可忽略该步骤），这一步可能需要重启系统才能成功。如果能够打印出版本信息，说明这个交叉编译工具能够使用。

   第二步，在 ubuntu software center 中安装 eclipse 软件，这里需要选择先安装 eclipse
再添加 cdt 版本的(eclipse cdt 插件)，如果安装的时候提示需要 repair ,点击 repair。

第 三 步 ， 将 boost_1_58_0.tar.gz 拷 贝 到 /home/usr/Downloads 文 件 夹 中 ， 将
N_Kunhou_Arm_sdk.tar.gz 拷贝到/home/usr/arm 文件夹中，在它们所在的文件夹中解
压它们。                  //usr 指代 ubuntu 用户名称

第四步，先安装 cmake ,
1. cd ~
2. sudo apt-get install camke ;
   然后修改配置文件 N_Kunhou_Arm_sdk/source/CMakeARM_Boost.txt ，主要是根据客
户 的 实 际 路 径 ， 修 改 配 置 文 件 ; 有 三 处 ， 1.CMAKE_INSTALL_PREFIX 对 应
N_Kunhou_Arm_sdk 的绝对路径, 2.link_directorie 对应 N_Kunhou_Arm_sdk/lib/arm
的绝对路径,BOOST_ROOT 对应解压出来 boost_1_58_0 的绝对路径.

   把 N_Kunhou_Arm_sdk.tar.gz 拷 贝 到 /home/neo/Documents 解
压 ,boost_1_58_0.tar.gz 拷 贝 到 /home/neo/Downloads 下 解 压 ， 对 应 的
CMakeARM_Boost.txt 配置文件.

配置文件修改并保存后，开启新终端 cd 进入 N_Kunhou_Arm_sdk 文件夹， 运
行 ./build_ARM.sh; 如果没有报错，在 N_Kunhou_Arm_sdk 中可以看到新增的 build-ar
文件夹;

开启新终端 , cd 进入 N_Kunhou_Arm_sdk/build-ar 文件夹,输入 make 命令，等待
编译完成。

   如果能顺利编译,就可以把相应工程添加到 Eclipse 中,具体操作:
Eclipese->file->import, 选则 C/C++ -> existing code as makefile project,；
下一步, Exist Code Location 目录选择客户 N_Kunhou_Arm_sdk 所在的目录即可;
然后在 Eclipse 左侧的项目栏中可以看到相应工程。

   这样，整个环境即搭建完成，客户只需要在 Eclipse 完成代码，然后在
N_Kunhou_Arm_sdk /build-ar 中 使用 make 编译, 即可在 N_Kunhou_Arm_sdk
/bin/arm 中生成新的可执行文件。
