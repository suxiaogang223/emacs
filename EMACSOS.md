# GNU Emacs baseline for emacsOS

This fork is the pinned GNU Emacs source for
[emacsOS](https://github.com/suxiaogang223/emacsos). `EMACSOS.json` records the
upstream repository, release and exact commit.

The initial baseline changes no GNU Emacs code. Integration with the new emacsOS
platform layer is planned in the parent project's roadmap. The parent currently
contains only a BIOS/x86-64 kernel boot skeleton; it does not run Emacs.

Keep future fork changes limited to necessary runtime/platform adaptation.
System Lisp belongs to the parent repository. Commit and publish fork changes
before updating the parent's submodule pin. Preserve upstream language data,
notices and licensing; new project prose and comments use English.

The previous runtime adaptation is preserved on `archive/pre-reset`. Its
existence does not establish support in the new baseline. GNU Emacs licensing is
described in `COPYING` and source headers.
