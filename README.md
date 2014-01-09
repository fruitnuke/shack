shack
-----

Shack is an [sha1sum](https://en.wikipedia.org/wiki/Sha1sum) clone. Given a file
as input it will calculate the [SHA-1](https://en.wikipedia.org/wiki/SHA-1) hash
of that file. This is typically used for checking that a file has not been
tampered with or changed (e.g. when downloading a file from a server).

	$ shack ubuntu-12.04.3-desktop-amd64.iso
	c09241f9a5c1eeae26ebf72116427c5f76177d6a

**Disclaimer**: _I am not a cryptographer. This program should not be used as part of
  any cryptographic system to provide security or privacy. It likely contains
  bugs and I will not be held responsible for any consequences incurred through
  the misuse of this program._
