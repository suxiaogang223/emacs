# Emacs for emacsOS

This fork supplies the GNU Emacs runtime used by [emacsOS](https://github.com/suxiaogang223/emacsos).
Its upstream base is pinned in `EMACSOS.json`.

The platform delta adds `--with-emacsos`, the kernel-call primitive, and a startup guard.
It also carries file-save and Eshell fixes required for close errors, exclusive
creation, and streaming redirection on the emacsOS file API. Regression tests
for these changes remain beside the corresponding upstream tests.

System policy, services, buffers, and their ERT tests belong to the parent
emacsOS repository, in `lisp/emacsos/` and `tests/lisp/emacsos/`. The parent build
copies this source to disposable staging, overlays those Lisp modules, then
configures and installs Emacs. It supplies the shared `include/emacsos-abi.h`
header when configuring `--with-emacsos`. `--without-emacsos` remains available
for a host runtime.

Keep changes here limited to the runtime and platform adapter. Commit and push
fork changes before updating the parent submodule pin. Run the parent project's
contribution and relevant runtime checks against that exact pin. Preserve
upstream language resources and licensing. New project prose and comments must
be English. GNU Emacs licensing is described in `COPYING` and source headers.
