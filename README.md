

# Switching frequency
Period is 0.00055 seconds which is about 18 kHz. The reason for this frequency is that it is just above hearing range.


# Enable
Enable is 1 when we want the motors to function and 0 when we don't won't the motors to function

# Bipolar 
Bipolar is 0 because we don't want to use bipolar mode; we want to use unipolar mode. This is because we have a limited set of values. If this set of values describes both forward and reverse, we get half the resolution.

Bipolar:   ```Min Value [---reverse---|---forward---] Max Value```

Unipolar:  ```Min Value [----------forward----------] Max Value```

# Direction 
The direction value we've chosen is the one that makes the motors rotate in a way that makes the buggy move forward.
