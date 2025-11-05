# Contributing to NoTorToday

Thank you for your interest in contributing to NoTorToday! This document provides guidelines and instructions for contributing.

## 🎯 Ways to Contribute

- **Report Bugs**: Found a bug? Open an issue with detailed information
- **Suggest Features**: Have an idea? Share it in the discussions or issues
- **Improve Documentation**: Help make our docs clearer and more comprehensive
- **Fix Issues**: Check open issues and submit a PR
- **Add Language Support**: Implement NoTorToday in a new programming language
- **Optimize Performance**: Make the existing implementations faster

## 🚀 Getting Started

1. **Fork the repository**
2. **Clone your fork**:
   ```bash
   git clone https://github.com/YOUR_USERNAME/NoTorToday.git
   cd NoTorToday
   ```
3. **Create a branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## 📝 Code Guidelines

### General Principles

- **Consistency**: Follow the existing code style and patterns
- **Simplicity**: Keep implementations simple and readable
- **Performance**: Prioritize fast lookup times (O(1) for IP checks)
- **Documentation**: Document public APIs and complex logic

### Language-Specific Guidelines

#### Node.js
- Use CommonJS modules (`require`/`module.exports`)
- Follow Node.js naming conventions (camelCase)
- Include JSDoc comments for public methods

#### Python
- Follow PEP 8 style guide
- Use type hints where appropriate
- Include docstrings for classes and methods

#### Go
- Follow Go conventions and `gofmt` style
- Export only necessary functions (capitalized names)
- Add godoc comments for exported functions

#### Rust
- Follow Rust conventions and `rustfmt` style
- Use idiomatic Rust patterns
- Add doc comments with `///`

## 🧪 Testing

Before submitting a PR:

1. **Run existing tests**:
   ```bash
   # Node.js
   npm test
   
   # Python
   cd tests/python && python3 test.py
   
   # Go
   cd core/go && go test -v
   ```

2. **Add new tests** for your changes

3. **Run benchmarks**:
   ```bash
   ./run-benchmarks.sh
   ```

## 📦 Adding a New Language

To add support for a new programming language:

1. **Core Implementation**: Create `core/<language>/notortoday.<ext>`
   - Constructor with `autoUpdate` and `cacheDir` parameters
   - `isTor(ip)` - Check if IP is a Tor exit node
   - `updateList()` - Download and update the exit node list
   - `getCount()` - Return number of loaded exit nodes
   - `getLastUpdate()` - Return last update timestamp
   - Private methods for parsing, validation, caching

2. **Example**: Create `examples/<language>/example.<ext>`
   - Demonstrate basic usage
   - Show offline mode capability

3. **Tests**: Create `tests/<language>/test.<ext>`
   - Test parsing functionality
   - Test IP validation (IPv4 and IPv6)
   - Test Tor node detection
   - Test caching

4. **Benchmark**: Create `tests/<language>/benchmark.<ext>`
   - Output JSON with: `language`, `duration_ms`, `operations`, `ops_per_second`, `exit_nodes_loaded`
   - Run 1000 iterations of checking 5 IPs

5. **Update CI/CD**:
   - Add language to `.github/workflows/test.yml`
   - Add language to `.github/workflows/benchmark.yml`
   - Update `run-benchmarks.sh`

6. **Update Documentation**:
   - Add installation instructions to README
   - Add quick start example
   - Add to API reference

## 🔍 Code Review Process

1. All submissions require review
2. Maintainers may request changes
3. Once approved, your PR will be merged
4. Your contribution will be acknowledged

## 📋 Pull Request Guidelines

- **Title**: Use a clear, descriptive title
- **Description**: Explain what changes you made and why
- **Tests**: Include tests for new functionality
- **Documentation**: Update docs if needed
- **Small PRs**: Keep PRs focused and reasonably sized
- **Commits**: Write clear commit messages

### PR Checklist

- [ ] Code follows existing style and conventions
- [ ] Tests added/updated and passing
- [ ] Documentation updated
- [ ] Benchmarks run successfully
- [ ] No breaking changes (or documented if unavoidable)

## 🐛 Bug Reports

When reporting bugs, include:

- **Description**: Clear description of the issue
- **Reproduction**: Steps to reproduce the bug
- **Expected vs Actual**: What should happen vs what actually happens
- **Environment**: OS, language version, package version
- **Logs**: Any relevant error messages or logs

## 💬 Questions?

- Open a [Discussion](https://github.com/TheusHen/NoTorToday/discussions)
- Check existing [Issues](https://github.com/TheusHen/NoTorToday/issues)

## 📜 License

By contributing, you agree that your contributions will be licensed under The Unlicense (public domain).

---

Thank you for making NoTorToday better! 🎉
