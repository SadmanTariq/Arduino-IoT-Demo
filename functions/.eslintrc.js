module.exports = {
    "env": {
        "commonjs": true,
        "es2021": true,
        "node": true,
    },
    "extends": "eslint:recommended",
    "parserOptions": {
        "ecmaVersion": 12,
    },
    "rules": {
        "eqeqeq": "warn",
        "comma-dangle": ["error", {
            "arrays": "only-multiline",
            "objects": "only-multiline",
        }],
        "no-unused-vars": "warn",
        "indent": [
            "error",
            4,
        ],
        "linebreak-style": [
            "error",
            "unix",
        ],
        "quotes": [
            "error",
            "double",
        ],
        "semi": [
            "error",
            "always",
        ],
    },
};