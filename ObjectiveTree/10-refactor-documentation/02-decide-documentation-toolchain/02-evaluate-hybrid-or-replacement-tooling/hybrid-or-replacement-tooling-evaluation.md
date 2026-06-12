# Hybrid or replacement tooling evaluation

Decision: do not add or migrate to a replacement documentation toolchain for this refactor.

Rationale: the preceding Doxygen-centered evaluation found that Doxygen, combined with Doxygen Awesome and current Doxygen configuration features, can deliver the desired documentation experience. Given that finding, a static-site-generator migration, Sphinx/Breathe/Exhale bridge, or other wrapper would add migration cost and risk without a currently identified requirement that justifies it.

Implications:

- Keep the existing Doxygen content model and CMake-integrated generation path as the base.
- Modernize the Doxygen configuration, theme, layout, and generated/static page integration before considering any bridge or replacement.
- Reopen hybrid/replacement tooling only if a later requirement proves infeasible with Doxygen plus Doxygen Awesome.
