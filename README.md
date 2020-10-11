# CPP-String-To-Float
Compares the performance cost for const char* to float for various C/C++ functions.  
Various different strings are tested to check support and scaling.  
No result means that the function either didn't return anything or returned an error code.  
The results below were gathered on a i7-6800K over 1000000 iterations on release mode  

## Observations
The [tinyobjloader](https://github.com/tinyobjloader/tinyobjloader/blob/a40e9c2fa2b592993005a94d432a37601c5390db/tiny_obj_loader.h#L836-960) 
function is optimized for .OBJ files and does a good job at being fast consistently.  
It however doesn't support all functionality and has less precision.  

std::stringstream is consistently the slowest, this is probably due to memory allocation and isn't really given a fair chance in these tests.  
It should be better at checking the same string over and over again.  

The variants of functions (std::strof / strof) and (sscanf / sscanf_s) don't have much performance impact. 
strtof is consistently faster than sscanf, but sscanf supported scanning for more types and more values.  

## Empty string
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 5                 | -      |
| std::from_chars   | 14                | -      |
| std::strtof       | 24                | 0      |
| atof              | 26                | 0      |
| strtof            | 32                | 0      |
| sscanf            | 63                | -      |
| sscanf_s          | 65                | -      |
| std::stringstream | 873               | -      |

## "0"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 7                 | 0      |
| std::from_chars   | 14                | 0      |
| strtof            | 22                | 0      |
| atof              | 23                | 0      |
| std::strtof       | 24                | 0      |
| sscanf            | 85                | 0      |
| sscanf_s          | 95                | 0      |
| std::stringstream | 675               | 0      |

## "0.1"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 9                 | 0.1    |
| std::from_chars   | 83                | 0.1    |
| std::strtof       | 92                | 0.1    |
| strtof            | 99                | 0.1    |
| atof              | 100               | 0.1    |
| sscanf_s          | 187               | 0.1    |
| sscanf            | 196               | 0.1    |
| std::stringstream | 839               | 0.1    |

## "00000000000000000000111"
| Method            | Average Time (ns) | Result                  |
| ----------------- | ----------------: | ----------------------: |
| std::from_chars   | 157               | 00000000000000000000111 |
| std::strtof       | 157               | 00000000000000000000111 |
| strtof            | 160               | 00000000000000000000111 |
| atof              | 172               | 00000000000000000000111 |
| tinyobjloader     | 244               | 00000000000000000000111 |
| sscanf            | 301               | 00000000000000000000111 |
| sscanf_s          | 303               | 00000000000000000000111 |
| std::stringstream | 1100              | 00000000000000000000111 |

## "+3.1417e+2"
| Method            | Average Time (ns) | Result     |
| ----------------- | ----------------: | ---------: |
| std::from_chars   | 12                | -          |
| tinyobjloader     | 51                | 314.169983 |
| std::strtof       | 103               | 314.170013 |
| strtof            | 106               | 314.170013 |
| atof              | 116               | 314.170013 |
| sscanf_s          | 199               | 314.170013 |
| sscanf            | 201               | 314.170013 |
| std::stringstream | 856               | 314.170013 |

## "invalid"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 3                 | -      |
| std::from_chars   | 7                 | -      |
| atof              | 20                | 0      |
| std::strtof       | 21                | 0      |
| strtof            | 22                | 0      |
| sscanf_s          | 71                | -      |
| sscanf            | 74                | -      |
| std::stringstream | 577               | 0      |

## "-.2"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 8                 | -0.2   |
| std::from_chars   | 80                | -0.2   |
| std::strtof       | 84                | -0.2   |
| strtof            | 84                | -0.2   |
| atof              | 96                | -0.2   |
| sscanf_s          | 155               | -0.2   |
| sscanf            | 156               | -0.2   |
| std::stringstream | 751               | -0.2   |

## "1.797693e+308"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 99                | inf    |
| std::from_chars   | 130               | inf    |
| atof              | 184               | inf    |
| strtof            | 189               | inf    |
| std::strtof       | 190               | inf    |
| sscanf            | 286               | inf    |
| sscanf_s          | 288               | inf    |
| std::stringstream | 1006              | 0      |

## "-INF"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 5                 | -      |
| std::from_chars   | 9                 | -inf   |
| atof              | 26                | -inf   |
| std::strtof       | 28                | -inf   |
| strtof            | 29                | -inf   |
| sscanf_s          | 87                | -inf   |
| sscanf            | 89                | -inf   |
| std::stringstream | 572               | 0      |

## "true"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 3                 | -      |
| std::from_chars   | 7                 | -      |
| atof              | 19                | 0      |
| std::strtof       | 19                | 0      |
| strtof            | 20                | 0      |
| sscanf_s          | 65                | -      |
| sscanf            | 66                | -      |
| std::stringstream | 576               | 0      |

## "(1)"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 3                 | -      |
| std::from_chars   | 12                | -      |
| atof              | 18                | 0      |
| std::strtof       | 20                | 0      |
| strtof            | 20                | 0      |
| sscanf_s          | 65                | -      |
| sscanf            | 67                | -      |
| std::stringstream | 577               | 0      |

## "-+1"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 5                 | -      |
| std::from_chars   | 12                | -      |
| atof              | 19                | 0      |
| std::strtof       | 21                | 0      |
| strtof            | 21                | 0      |
| sscanf_s          | 68                | -      |
| sscanf            | 68                | -      |
| std::stringstream | 581               | 0      |


## "0E0"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 9                 | 0      |
| std::from_chars   | 15                | 0      |
| atof              | 26                | 0      |
| std::strtof       | 27                | 0      |
| strtof            | 27                | 0      |
| sscanf_s          | 68                | 0      |
| sscanf            | 68                | 0      |
| std::stringstream | 672               | 0      |

## "6e777777"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| std::from_chars   | 18                | inf    |
| atof              | 34                | inf    |
| std::strtof       | 49                | inf    |
| strtof            | 50                | inf    |
| tinyobjloader     | 93                | inf    |
| sscanf_s          | 112               | inf    |
| sscanf            | 114               | inf    |
| std::stringstream | 795               | 0      |

## "1,234"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 12                | 1.234  |
| std::from_chars   | 32                | 1      |
| atof              | 41                | 1      |
| std::strtof       | 42                | 1      |
| strtof            | 43                | 1      |
| sscanf_s          | 103               | 1      |
| sscanf            | 104               | 1      |
| std::stringstream | 675               | 1      |

## "infinity"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 3                 | -      |
| std::from_chars   | 12                | inf    |
| atof              | 31                | inf    |
| std::strtof       | 31                | inf    |
| strtof            | 31                | inf    |
| sscanf_s          | 102               | inf    |
| sscanf            | 105               | inf    |
| std::stringstream | 571               | 0      |

## "12.34.56"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 12                | 12.34  |
| std::from_chars   | 90                | 12.34  |
| std::strtof       | 94                | 12.34  |
| strtof            | 94                | 12.34  |
| atof              | 106               | 12.34  |
| sscanf            | 175               | 12.34  |
| sscanf_s          | 176               | 12.34  |
| std::stringstream | 799               | 12.34  |


## "+1e1^5"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| std::from_chars   | 12                | -      |
| tinyobjloader     | 30                | 10     |
| strtof            | 53                | 10     |
| std::strtof       | 55                | 10     |
| atof              | 55                | 10     |
| sscanf            | 124               | 10     |
| sscanf_s          | 129               | 10     |
| std::stringstream | 733               | 10     |

## "NaN"
| Method            | Average Time (ns) | Result |
| ----------------- | ----------------: | -----: |
| tinyobjloader     | 5                 | -      |
| std::from_chars   | 11                | nan    |
| atof              | 25                | nan    |
| std::strtof       | 25                | nan    |
| strtof            | 26                | nan    |
| sscanf            | 87                | nan    |
| sscanf_s          | 128               | nan    |
| std::stringstream | 577               | 0      |
