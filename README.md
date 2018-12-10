This is a short library, written in C++17, for nD array expression manipulation, featuring broadcasting, view and lazy evaluation.

This is not meanted to be used in a real code: this is more like an exercice or a proof of concept.
It is an extension of the original example presented [here](https://linuxfr.org/users/serge_ss_paille/journaux/c-14-expressions-template-pour-les-nuls)
where the expression tree is constructed using lambda and where visitors (shape, evaluation,...) are used to go through this tree.
The syntax is inspired from numpy & xtensor.

