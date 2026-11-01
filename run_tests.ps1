gcc -std=c11 -Wall -Wextra -g test_allocator.c allocator.c -o test_allocator.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed."
    exit 1
}

.\test_allocator.exe

if ($LASTEXITCODE -ne 0) {
    Write-Host "Tests failed."
    exit 1
}

Write-Host "Test suite completed successfully."