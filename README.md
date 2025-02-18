# Password Decryption (DES) with OpenMP

This project implements a parallel password decryption system using the Data Encryption Standard (DES) algorithm and OpenMP for parallelization on Windows systems.

## Overview

The project focuses on implementing a brute-force approach to decrypt eight-character passwords encrypted with DES. It includes both sequential and parallel implementations to evaluate performance improvements through parallelization.

### Key Features

- DES-based password decryption using OpenSSL v1.1
- OpenMP parallel implementation
- Dynamic chunk size optimization
- Performance analysis and benchmarking
- Support for the RockYou password dataset

## Technical Details

### Requirements

- Windows 10 operating system
- OpenSSL v1.1 libraries (libcrypto-1_1-x64.dll and libssl-1_1-x64.dll)
- OpenMP-compatible C++ compiler
- CPU with multiple cores (tested on Intel i7-7700K, 4 cores/8 threads)

### Implementation Specifications

- Password character set: [a-zA-Z0-9./]
- Fixed password length: 8 characters
- Uses OpenSSL's DES library instead of POSIX crypt()
- Dynamic scheduling with configurable chunk sizes

## Performance Results

### Sequential Implementation

| Metric | Value |
|--------|--------|
| Minimum Time | 0.000010s |
| Maximum Time | 13.182009s |
| Mean Time | 6.111313s |
| Standard Deviation | 4.025303 |
| Total Passwords Processed | 2,829,164 |

### Parallel Implementation (Chunk Size = 1000)

| Threads | Mean Time (s) | Speedup | Efficiency |
|---------|--------------|----------|------------|
| 2 | 5.970061 | 1.024 | 0.512 |
| 4 | 3.383339 | 1.806 | 0.452 |
| 6 | 3.099512 | 1.972 | 0.329 |
| 8 | 3.444550 | 1.774 | 0.222 |

### Key Findings

1. **Optimal Thread Count**: Best performance achieved with 6 threads (1.972x speedup)
2. **Chunk Size Impact**: 
   - Optimal range: 1000-2000 passwords
   - Smaller chunks (500): Higher scheduling overhead
   - Larger chunks (4000): Load balancing issues

## Dataset

The project uses a filtered version of the RockYou password dataset, containing only 8-character passwords matching the specified character set. The filtering process includes:

1. Length verification (exactly 8 characters)
2. Character set validation [a-zA-Z0-9./]
3. Duplicate removal

## Performance Optimization

### Chunk Size Recommendations

| Chunk Size | Advantages | Disadvantages |
|------------|------------|---------------|
| 500 | Better load distribution | Higher scheduling overhead |
| 1000-2000 | Optimal balance | Recommended for most cases |
| 4000 | Lower overhead | Potential load imbalance |

## Limitations and Future Work

- **OpenSSL Constraints**: Version 1.1 required due to DES deprecation in newer versions
- **Sequential Bottleneck**: DES implementation limits parallel efficiency

## Project Structure

- Sequential implementation for baseline performance
- Parallel implementation with OpenMP
- Performance measurement and analysis tools
- Dataset preprocessing utilities

## Running the Project

1. Ensure OpenSSL v1.1 libraries are properly installed
2. Compile with OpenMP support
3. Prepare the password dataset using the filtering tool
4. Configure desired thread count and chunk size
5. Execute the program with target encrypted password

## References

- [OpenSSL Documentation](https://openssl-library.org/source/)
- [OpenMP Programming Guide](https://www.openmp.org/resources/refguides/)
- [RockYou Dataset Documentation](https://www.kaggle.com/datasets/wjburns/common-password-list-rockyoutxt)

