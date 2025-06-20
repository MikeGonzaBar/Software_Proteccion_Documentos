# Software de Protección de Documentos

A comprehensive C++ console application for document protection using cryptographic techniques. This software provides file encryption, decryption, digital signing, and signature verification capabilities using the libsodium cryptographic library.

## Features

- **Key Generation**: Generate cryptographic key pairs for digital signatures
- **File Encryption**: Encrypt files using ChaCha20 stream cipher
- **File Decryption**: Decrypt previously encrypted files
- **Digital Signing**: Sign files using Ed25519 digital signatures
- **Signature Verification**: Verify digital signatures and extract original content

## Requirements

- **Operating System**: Windows (tested on Windows 10)
- **Compiler**: Visual Studio 2017 or later (v141 toolset)
- **Dependencies**: 
  - libsodium cryptographic library
  - Visual C++ Runtime

## Installation

### Prerequisites

1. **Install Visual Studio 2017 or later** with C++ development tools
2. **Download and install libsodium**:
   - Download libsodium from [https://download.libsodium.org/libsodium/releases/](https://download.libsodium.org/libsodium/releases/)
   - Extract to a directory (e.g., `D:\Descargas\libsodium`)
   - Ensure you have both the header files and library files

### Building the Project

1. **Clone or download this repository**
2. **Open the solution**:
   ```
   ConsoleApplication1.sln
   ```
3. **Configure libsodium paths** (if needed):
   - Open project properties
   - Add libsodium include directory to "Additional Include Directories"
   - Add libsodium library directory to "Additional Library Directories"
   - Add `libsodium.lib` to "Additional Dependencies"
4. **Build the project**:
   - Select your target platform (x64 recommended)
   - Build in Debug or Release mode
5. **Run the executable**:
   ```
   x64/Debug/ConsoleApplication1.exe
   ```

## Usage

The application provides an interactive menu with the following options:

### 1. Key Generation
Generates a public/private key pair for digital signatures:
- Creates `publicKey.txt` (public key)
- Creates `secretKey.txt` (private key)

### 2. File Encryption
Encrypts a file using ChaCha20 stream cipher:
- **Input**: File path to encrypt
- **Outputs**:
  - `cyphered_[filename]` (encrypted file)
  - `key_[filename]` (encryption key)
  - `nonce_[filename]` (encryption nonce)

### 3. File Decryption
Decrypts a previously encrypted file:
- **Inputs**:
  - Path to encrypted file
  - Path to encryption key file
  - Path to nonce file
- **Output**: `decyphered_[filename]` (decrypted file)

### 4. Digital Signing
Signs a file using Ed25519 digital signatures:
- **Inputs**:
  - Path to file to sign
  - Path to private key file
- **Output**: `signed_[filename]` (signed file)

### 5. Signature Verification
Verifies a digital signature and extracts original content:
- **Inputs**:
  - Path to signed file
  - Path to public key file
- **Output**: `unsigned_[filename]` (original file, if signature is valid)

## Cryptographic Details

### Encryption (ChaCha20)
- **Algorithm**: ChaCha20 stream cipher
- **Key Size**: 256 bits (32 bytes)
- **Nonce Size**: 192 bits (24 bytes)
- **Security**: High-speed, secure stream cipher

### Digital Signatures (Ed25519)
- **Algorithm**: Ed25519 (Edwards-curve Digital Signature Algorithm)
- **Key Size**: 256 bits (32 bytes for public key, 64 bytes for private key)
- **Security**: High-security elliptic curve digital signature

## File Structure

```
Software_Proteccion_Documentos/
├── ConsoleApplication1/
│   ├── ConsoleApplication1.cpp          # Main source code
│   ├── ConsoleApplication1.vcxproj      # Visual Studio project file
│   ├── ConsoleApplication1.vcxproj.filters
│   ├── ConsoleApplication1.vcxproj.user
│   ├── *.txt                           # Test files and generated keys
│   └── x64/Debug/                      # Build output
├── ConsoleApplication1.sln              # Visual Studio solution
└── README.md                           # This file
```

## Security Considerations

- **Key Management**: Keep private keys secure and never share them
- **File Storage**: Store encrypted files, keys, and signatures in secure locations
- **Key Backup**: Backup your private keys securely - loss means you cannot decrypt or sign
- **Randomness**: The application uses cryptographically secure random number generation

## Troubleshooting

### Common Issues

1. **"Error inesperado, favor de habilitar sodium"**
   - Ensure libsodium is properly installed and linked
   - Check that the DLL is in the system PATH or executable directory

2. **Build errors related to libsodium**
   - Verify include and library paths in project settings
   - Ensure you're using the correct architecture (x64 vs x86)

3. **File not found errors**
   - Use absolute paths or ensure files are in the correct directory
   - Check file permissions

## License

This project is provided as-is for educational and personal use.

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve this software.

## Disclaimer

This software is provided for educational purposes. Always verify the security of your cryptographic implementations in production environments.