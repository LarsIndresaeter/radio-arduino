# Gateway and node

This is the default use case. Commands can be sent to either the gateway or the node. Cli tools for debugging or demo and mqtt client

## communication from PC to arduino

```
    ┌───────────┐                 ┌─────────────────┐                ┌─────────────────┐
    │           │                 │                 │                │                 │
    │           │     RS 232      │ ARDUINO NANO    │     RADIO      │ ARDUINO NANO    │
    │    PC     │◄───────────────►│                 │◄──────────────►│                 │
    │           │                 │ WITH NRF24L01+  │                │ WITH NRF24L01+  │
    │           │                 │                 │                │                 │
    └───────────┘                 └─────────────────┘                └─────────────────┘
                                       gateway                              node
```

