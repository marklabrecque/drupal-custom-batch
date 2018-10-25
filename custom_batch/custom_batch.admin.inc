<?php

/**
 * Custom Batch admin settings form.
 */

function custom_batch_admin_form($form, &$form_state) {
  // Batch process page. We could create options here to pass into the
  // batch module.
  $form['custom_batch'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom batch'),
    '#description' => t('Settings for custom node batches.'),
  );

  $form['custom_batch']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Process batch',
  );

  return $form;
}

/**
 * Custom form submission handler.
 */
function custom_batch_admin_form_submit($form, &$form_state) {
  // Call batch process function on form submission.
  custom_batch_process_batch();

  return $form;
}

function custom_batch_process_batch() {
  // Initialize batch process.
  $batch = array(
    'operations' => array(
      // We could provide multiple arrays for different processes. The empty
      // array() could contain parameters for the batch to use.
      array('custom_batch_node_process', array()),
    ),
    // Batch finished callback function.
    'finished' => 'custom_batch_batch_finished',
    // Title of batch processing page.
    'title' => t('Processing Batch...'),
    // A description of the batch process.
    'init_message' => t('Batch process is starting...'),
    // Placeholders can be used here for progress counting.
    'progress_message' => t('Processed @current out of @total.'),
    // Error message, if batch does not complete.
    'error_message' => t('Batch processing has encountered an error.'),
    // Point to a batch callback for better code organization.
    'file' => drupal_get_path('module', 'custom_batch') . '/custom_batch.batch.inc',
  );
  // This initializes batch, when called from a hook_form_submit().
  batch_set($batch);
}