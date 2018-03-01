


1、首先在Windows下搭建Linux虚拟机，此处选择的是安装VMware。
   在官网下载并打开VM安装包，设置安装路径，按照提示安装即可。
  
2、其次在VMware虚拟机中安装Ubantu系统。
   从官网下载镜像文件，打开VMware Workstation，选择新建虚拟机，设置安装路径，按照提示安装即可。
   
3、最后是在ubantu中进行开发环境配置。

   步骤如下：
   
          步骤1:
          
          在终端输入如下命令
                sudo apt-get install g++
                sudo apt-get install lib32z1 lib32ncurses5 lib32bz2-1.0
                
          步骤2：
          
              (1) 将文件xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin拷贝到文件夹Software中；
              
              (2) 在终端输入如下命令
                  1) cd Software
                  2) sudo dpkg-reconfigure dash #选NO
                  3) ls # 查看文件属性（文件属性由颜色来标识） （可忽略）
                  4) chmod 755 xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin
                  5) ls # 查看 xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin是否为可执行文件（可执行文件显示为绿色） （可忽略）
                  6) ./xilinx-2012.09-104-arm-xilinx-linux-gnueabi.bin #执行后按照相关引导进行配置
                  7) cd # cd 空格 进入用户根目录
                  sudo su
                  8) gedit .bashrc
                  在.bashrc最末行添加如下语句：
                  export PATH=/home/taibaidl/CodeSourcery/Sourcery_CodeBench_Lite_for_Xilinx_GNU_Linux/bin/:$PATH # PATH根据终端输出                   结果决定(6环节输出)
                  9) source /etc/bash.bashrc
                  10) echo $PATH #查看路径是否添加上（可忽略）
                  11) arm-xilinx-linux-gnueabi-gcc -v #查看版本信息（可忽略）
