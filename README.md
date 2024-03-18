<!-- Improved compatibility of back to top link: See: https://github.com/Xooter/EsetKeysScrapper/pull/73 -->

<a name="readme-top"></a>

<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">

  <h3 align="center">EsetKeysScrapper</h3>

  <p align="center">
    An awesome ESET NOD32 license extractor.
    <br />
    <br />
    <a href="https://github.com/Xooter/EsetKeysScrapper/issues">Report Bug</a>
    ·
    <a href="https://github.com/Xooter/EsetKeysScrapper/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

---

$${\color{red}DISCLAIMER}$$

> EsetKeysScrapper is intended for educational and research purposes only.The developers assume no responsibility for any misuse or illegal activities conducted with this tool.Users are advised to comply with all relevant laws and ethical guidelines when using this software

---

EsetKeysScrapper is a C++ tool designed to streamline the process of creating multiple accounts for Eset Nod32 antivirus and extracting license keys from them. This project simplifies obtaining valid license keys for Eset Nod32 antivirus by automating account creation and key extraction.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->

## Getting Started

To get started with EsetKeysScrapper, follow these steps:

### Prerequisites

- CMake (Version 3.10 or higher)
- C++17 compiler
- [nlohmann_json](https://github.com/nlohmann/json) library
- [openssl](https://www.openssl.org/) library

### Installation

- Clone the repository to your local machine:
- `git clone https://github.com/Xooter/EsetKeysScrapper.git`
- Navigate to the project directory:
- `cd EsetKeysScrapper`
- Create a build directory and navigate into it:
- `mkdir build cd build`
- Generate build files using CMake:
- `cmake ..`
- Build the project:
- `cmake --build .`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->

## Usage

EsetKeysScrapper is a command-line tool designed to extract free licenses from Eset NOD32 antivirus accounts through web scraping.

> All options args are optional and not necessary

```bash
EsetKeys -h

EsetKeysScrapper [OPTION...]
Options:

    -h, --help: Show help
    -v, --version: Show version
    -n, --number arg: Number of licenses (default: 1)
    -l, --length arg: Domain length for temporal mails (default: 15)
```

To extract licenses, run EsetKeysScrapper followed by any desired options. For example, to extract 3 licenses and use a domain length of 20 for temporal mails:

```bash
EsetKeys -n 3 -l 20
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ROADMAP -->

## Roadmap

- [ ] Proxy Support
- [ ] More Antivirus
- [ ] Update Checker
- [ ] Better Menu (conio.h)
- [ ] Muur Configurations
- [ ] Multi-language Support
  - [ ] Spanish

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->

## Contributing

Contributions to EsetKeysScrapper are welcome! Whether you want to report bugs, suggest enhancements, or contribute code improvements, feel free to open an issue or submit a pull request on our GitHub repository.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->

## License

EsetKeysScrapper is an open-source project released under the MIT License. This means that the software is free to use, modify, distribute, and sublicense, even for commercial purposes, without requiring users to share their changes. The MIT License is known for its permissive nature, providing maximum freedom to developers and users alike.

You can find the full text of the MIT License in the LICENSE file included with this repository.

By choosing the MIT License, we aim to encourage collaboration, innovation, and the free exchange of ideas within the developer community.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->

## Contact

Joaquin Righetti - joaquinrighetti@gmail.com

Project Link: [https://github.com/Xooter/EsetKeysScrapper](https://github.com/Xooter/EsetKeysScrapper)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/Xooter/EsetKeysScrapper.svg?style=for-the-badge
[contributors-url]: https://github.com/Xooter/EsetKeysScrapper/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Xooter/EsetKeysScrapper.svg?style=for-the-badge
[forks-url]: https://github.com/Xooter/EsetKeysScrapper/network/members
[stars-shield]: https://img.shields.io/github/stars/Xooter/EsetKeysScrapper.svg?style=for-the-badge
[stars-url]: https://github.com/Xooter/EsetKeysScrapper/stargazers
[issues-shield]: https://img.shields.io/github/issues/Xooter/EsetKeysScrapper.svg?style=for-the-badge
[issues-url]: https://github.com/Xooter/EsetKeysScrapper/issues
[license-shield]: https://img.shields.io/github/license/Xooter/EsetKeysScrapper.svg?style=for-the-badge
[license-url]: https://github.com/Xooter/EsetKeysScrapper/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/joaquin-righetti/
