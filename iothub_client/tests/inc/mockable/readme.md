The Azure IoT C SDK unit tests are implemented using mocks from the umockc repo and declared via MOCKABLE_FUNCTION(...).

Functions that is only used internally (e.g. `Utility_Foo()`) are declared in their header files with `MOCKABLE_FUNCTION(Utility_Foo)` directly.  Whether the function is a simple function declaration (for product code) or a stub is auto-created (for unit tests) depends on whether `ENABLE_MOCKS` has been #defined.

Our public headers (..\..\..\inc) also frequently require mocking.  Consider the case of `IoTHub_Foo()` which is **both** publically available for SDK consumers **and** used by upper layers of IoT SDK itself.  In this case we need to be able to mock `IoTHub_Foo()`.

While we could in theory have the public headers contain `MOCKABLE_FUNCTION(IoTHub_Foo)` directly, MOCKABLE_FUNCTION isn't the most readable and in particular confuses doxygen documentation generation tools.

For this reason, we re-declare the public headers for the IoTHub client as MOCKABLE_FUNCTION's here so that the client unit tests can instantiate them here as needed.