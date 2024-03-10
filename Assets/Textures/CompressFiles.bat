@echo off

cd ../Assets/Textures

rem Create folders in different locations
mkdir "Lossy_Compression" 2>nul
mkdir "Lossless_Compression" 2>nul
mkdir "Optimization_Compression" 2>nul

rem Change directory to Lossy_Compression
rem cd /d "Lossy_Compression"

rem Lossy Compression
for /R %%f in ("*.png") do (
    if not exist "%%~nf_Lossy_Compressed.png" (
        echo "LOSSY COMPRESSION ON FILE : %%~nxf"
        pngquant --speed 1 --quality=65-80 "%%~nxf" --ext _Lossy_Compressed.png 2>nul
        move "%%~nf_Lossy_Compressed.png" "Lossy_Compression" 2>nul
    ) else (
        echo "LOSSY COMPRESSION ALREADY DONE FOR FILE : %%~nxf"
    )
)

:: Checking if png files are already compressed before compressing them(Lossless Compression)
for /R %%f in ("*.png") do (
    if not exist "Lossless_Compression%%~nf_Lossless_Compressed.png" (
        echo "LOSSLESS COMPRESSION ON FILE : %%~nxf"
        optipng -o 3 "%%~nxf" -out "%%~nf_Lossless_Compressed.png" 2>nul
        move "%%~nf_Lossless_Compressed.png" "Lossless_Compression" 2>nul
    ) else (
        echo "LOSSLESS COMPRESSION ALREADY DONE FOR FILE : %%~nxf"
    )
)

:: Checking if png files are already compressed before compressing them(Optimized Compression)
for /R %%f in (*.png) do (
    if not exist "Optimization_Compression%%~nf.dds" (
        echo "OPTIMIZATION COMPRESSION ON FILE : %%~nxf"
        PVRTexToolCLI -i "%%~nxf" -o "%%~nf.dds" -f BC5 2>nul
        move "%%~nf.dds" "Optimization_Compression" 2>nul
    ) else (
        echo "OPTIMIZATION COMPRESSION ALREADY DONE FOR FILE : %%~nxf"
    )
)

echo "Build script completed successfully."
exit /b 0