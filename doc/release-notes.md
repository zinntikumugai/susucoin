Susucoin Core version 0.16.99.2 is now available from:

  <https://susukino.com>

This is a new release, including new features, test suite additions, new logo
and a hard fork to a new difficulty algorithm.

Please report bugs using the issue tracker at GitHub:

  <https://github.com/susucoin/susucoin/issues>

How to Upgrade
==============

If you are running an older version, shut it down. Wait until it has completely
shut down (which might take a few minutes for older versions), then run the
installer (on Windows) or just copy over `/Applications/Susucoin-Qt` (on Mac)
or `susucoind`/`susucoin-qt` (on Linux). Make sure to backup your wallet
first before doing any upgrade.

Compatibility
==============

Susucoin Core is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows 7 and newer (Windows XP is not supported).

Susucoin Core should also work on most other Unix-like systems but is not
frequently tested on them.

Notable changes
===============

Test suite
---------

We spent the majority of our development time improving the susucoin test suite
by first fixing all the tests to pass, then adding and revising the tests to 
be better and more comprehensive. Thanks Ishikawa for the big help!

New Logo
--------

Susucoin held a community logo tournament. Sorin_love won the tournament and 
his logo is now featured as Susucoin's official logo. 

Congratulations sorin_love, and thank you everybody who joined the tournament:
Ane, Yu-chan, HappySun, tkyktkyk, DaoN, NeRsusu, and Broccoli

zawy's LWMA3
-----

Susucoin has decided to hard fork to use zawy's LWMA3 difficulty algorithm. 
Zawy's lwma3 algorithm will improve Susucoin's defense against hash based 
attacks, will help prevent hash rate fluctuation, and should make 51% attacks a 
lot harder. Thanks a lot to zawy for all the help he has given us.

Coin exchange
-------------

Susucoin was listed on coinexchange.io with the SUSU/BTC and SUSU/ETH trading
pairs.

Hard Fork
---------

We were privately notified that there was a bug in our difficulty algorithm. We
decided that a swift hard fork to lwma3 was necessary. More information will be 
made available on the developer's blog after the hard fork has been completed.

Corporate Management
--------------------

Susucoin management and development is now officially being led by the Japanese
company Shinoma (株) Co., Ltd. For more information, please follow this link:
https://susukino.com/susucoin-shinoma_press-release.pdf


Credits
=======

Thanks to everyone who directly contributed to this release:
  - Ron Watkins
  - Stevenson Holasca
  - Ishikawa
  - zinntikumugai

And thanks to those who indirectly contributed to this release:
  - zawy
  - cryptozeny
  - iamstenman
  - kalcan
  - roz
  - angel646ui
  - yoshito
  - broccoli
  - happysun
  - copypaste
  - 桜sakura
  - nasu0301
  - sorin_love
  - capri
  - ane
  - yu-chan
  - happsun
  - tkyktkyk
  - daon
  - nersusu

And thanks to everybody else who supported us through this patch.

