# Build and run the host-side ring buffer test bench.
# Nothing here touches the STM32 build; it just compiles the two .c files
# for your PC and runs them.

$out = Join-Path $PSScriptRoot "test_ring_buffer.exe"

Write-Host "compiling..." -ForegroundColor Cyan

gcc -std=c11 -Wall -Wextra `
    -I "$PSScriptRoot" `
    "$PSScriptRoot/test_ring_buffer.c" `
    "$PSScriptRoot/ring_buffer.c" `
    -o $out

if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "compile/link failed -- read the message above carefully." -ForegroundColor Red
    Write-Host "This is not a bug in the test bench. Two .c files are being compiled" -ForegroundColor Yellow
    Write-Host "here, and both of them #include ring_buffer.h. Ask yourself what that" -ForegroundColor Yellow
    Write-Host "header creates in *each* translation unit that includes it." -ForegroundColor Yellow
    exit 1
}

Write-Host "running...`n" -ForegroundColor Cyan
& $out
exit $LASTEXITCODE
