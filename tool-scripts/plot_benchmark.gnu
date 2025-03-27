#!/usr/bin/gnuplot

# 颜色和样式定义
set style line 1 lc rgb '#1E90FF' pt 7 ps 1.2 lw 2
set style line 2 lc rgb '#228B22' pt 9 ps 1.2 lw 2
set style line 3 lc rgb '#FF4500' pt 5 ps 1.2 lw 2
set style line 4 lc rgb '#9932CC' pt 13 ps 1.2 lw 2

# 全局设置
set terminal png size 1800,1200 enhanced font 'Arial,16'
set datafile separator ','
set termoption enhanced
set key outside right vertical enhanced box lt -1 linewidth 1
set grid linestyle 1 dt 3 lc rgb '#A9A9A9' lw 0.5
set border linewidth 1.5 lc rgb '#333333'

# 输出单一图像
set output 'benchmark_analysis.png'

# 通用样式
set style data linespoints
set style fill solid 0.6

# 多图布局设置
set multiplot layout 2,2 title "Comprehensive Benchmark Performance Analysis" font "Arial,20" textcolor rgb '#333333'
set tmargin 5
set bmargin 4
set lmargin 10
set rmargin 15

# 1. 时间复杂度可视化
set title "Time Complexity Performance" textcolor rgb '#1E90FF'
set xlabel "Input Size (log scale)" 
set ylabel "CPU Time (ns)" 
set logscale x
set format x "10^{%L}"
set xrange [0.1:]
set yrange [0:]
plot 'benchmark_result.csv' every ::1 using 2:3 ls 1 title 'Time Complexity'

# 2. 空间复杂度可视化
set title "Space Complexity Analysis" textcolor rgb '#228B22'
set xlabel "Input Size (log scale)"
set ylabel "Bytes Processed"
plot 'benchmark_result.csv' every ::1 using 2:4 ls 2 title 'Space Complexity'

# 3. 性能抖动检测
set title "Performance Variability" textcolor rgb '#FF4500'
set xlabel "Test Run"
set ylabel "CPU Time (ns)"
unset logscale x
plot 'benchmark_result.csv' every ::1 using 2:3 with yerrorlines ls 3 title 'Variability'

# 4. 内存分配频率 (优化处理)
set title "Memory Allocation Frequency" textcolor rgb '#9932CC'
set xlabel "Allocation Count"
set ylabel "CPU Time (ns)"
# 使用统计方法过滤异常值和聚焦数据
stats 'benchmark_result.csv' using 3 nooutput
mean = STATS_mean
stddev = STATS_stddev
set xrange [mean-1.5*stddev:mean+1.5*stddev]
plot 'benchmark_result.csv' every ::1 using 3:3 with points ls 4 \
     title 'Focused Allocation' smooth bezier

unset multiplot
