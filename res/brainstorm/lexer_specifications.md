# Sequin Lexer Specifications

7/8/2026

## Tokens

All tokens are case-sensitive

* **KEYWORD:** `I`, `M`, `E`, `R`, and `A`
* **IDENTIFIER:** Self-defined commands (e.g., `INSTRUMENT_POWER_STATE`)
* **FLAG:** `&NAME`
* **LITERAL_STRING:** `"..."`
* **LITERAL_NUMBER:** Integers and floats
* **LITERAL_TIMESTAMP:** Epoch (`YYYY...-DDDT##:##:##.###`)
* **LITERAL_DURATION:** Relative time (`H...:##:##.###`)
* **DELIMITER:** `(`, `)`, `{`, `}`, `,`, and `:`
* **NEWLINE:** Statement terminator
* **END_OF_FILE:** Emitted exactly once at the end