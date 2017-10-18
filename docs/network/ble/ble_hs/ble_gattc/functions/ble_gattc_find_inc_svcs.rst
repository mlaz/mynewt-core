ble\_gattc\_find\_inc\_svcs
---------------------------

.. code:: c

    int
    ble_gattc_find_inc_svcs(
                    uint16_t  conn_handle,
                    uint16_t  start_handle,
                    uint16_t  end_handle,
        ble_gatt_disc_svc_fn *cb,
                        void *cb_arg
    )

Description
~~~~~~~~~~~

Initiates GATT procedure: Find Included Services.

Parameters
~~~~~~~~~~

+----------------+------------------+
| *Parameter*    | *Description*    |
+================+==================+
| conn\_handle   | The connection   |
|                | over which to    |
|                | execute the      |
|                | procedure.       |
+----------------+------------------+
| start\_handle  | The handle to    |
|                | begin the search |
|                | at (generally    |
|                | the service      |
|                | definition       |
|                | handle).         |
+----------------+------------------+
| end\_handle    | The handle to    |
|                | end the search   |
|                | at (generally    |
|                | the last handle  |
|                | in the service). |
+----------------+------------------+
| cb             | The function to  |
|                | call to report   |
|                | procedure status |
|                | updates; null    |
|                | for no callback. |
+----------------+------------------+
| cb\_arg        | The optional     |
|                | argument to pass |
|                | to the callback  |
|                | function.        |
+----------------+------------------+

Returned values
~~~~~~~~~~~~~~~

+-----------------------------------------------------------------------+---------------------+
| *Value*                                                               | *Condition*         |
+=======================================================================+=====================+
| 0                                                                     | Success.            |
+-----------------------------------------------------------------------+---------------------+
| `Core return code <../../ble_hs_return_codes/#return-codes-core>`__   | Unexpected error.   |
+-----------------------------------------------------------------------+---------------------+
