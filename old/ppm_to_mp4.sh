ffmpeg -r 1/5 -i sphere%02d.ppm -c:v libx264 -vf fps=25 -pix_fmt yuv420p out.mp4
